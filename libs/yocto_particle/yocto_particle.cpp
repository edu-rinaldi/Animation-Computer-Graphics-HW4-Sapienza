//
// Implementation for Yocto/Particle.
//

//
// LICENSE:
//
// Copyright (c) 2020 -- 2020 Fabio Pellacini
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//

#include "yocto_particle.h"

#include <yocto/yocto_shape.h>

#include <unordered_set>

// -----------------------------------------------------------------------------
// ALIASES
// -----------------------------------------------------------------------------
namespace yocto::particle {

// import math symbols for use
using math::abs;
using math::acos;
using math::atan;
using math::atan2;
using math::clamp;
using math::cos;
using math::exp;
using math::exp2;
using math::flt_max;
using math::fmod;
using math::log;
using math::log2;
using math::make_rng;
using math::max;
using math::min;
using math::perspective_mat;
using math::pow;
using math::ray3f;
using math::sin;
using math::sqrt;
using math::tan;

}  // namespace yocto::particle

// -----------------------------------------------------------------------------
// SIMULATION DATA AND API
// -----------------------------------------------------------------------------
namespace yocto::particle {

// cleanup
scene::~scene() {
  for (auto shape : shapes) delete shape;
  for (auto collider : colliders) delete collider;
}

// Scene creation
par::shape* add_shape(par::scene* scene) {
  return scene->shapes.emplace_back(new par::shape{});
}
par::collider* add_collider(par::scene* scene) {
  return scene->colliders.emplace_back(new par::collider{});
}
par::shape* add_particles(par::scene* scene, const std::vector<int>& points,
    const std::vector<vec3f>& positions, const std::vector<float>& radius,
    float mass, float random_velocity) {
  auto shape               = add_shape(scene);
  shape->points            = points;
  shape->initial_positions = positions;
  shape->initial_normals.assign(shape->positions.size(), {0, 0, 1});
  shape->initial_radius = radius;
  shape->initial_invmass.assign(
      positions.size(), 1 / (mass * positions.size()));
  shape->initial_velocities.assign(positions.size(), {0, 0, 0});
  shape->emit_rngscale = random_velocity;
  return shape;
}
par::shape* add_cloth(par::scene* scene, const std::vector<vec4i>& quads,
    const std::vector<vec3f>& positions, const std::vector<vec3f>& normals,
    const std::vector<float>& radius, float mass, float coeff,
    const std::vector<int>& pinned) {
  auto shape               = add_shape(scene);
  shape->quads             = quads;
  shape->initial_positions = positions;
  shape->initial_normals   = normals;
  shape->initial_radius    = radius;
  shape->initial_invmass.assign(
      positions.size(), 1 / (mass * positions.size()));
  shape->initial_velocities.assign(positions.size(), {0, 0, 0});
  shape->initial_pinned = pinned;
  shape->spring_coeff   = coeff;
  return shape;
}
par::collider* add_collider(par::scene* scene,
    const std::vector<vec3i>& triangles, const std::vector<vec4i>& quads,
    const std::vector<vec3f>& positions, const std::vector<vec3f>& normals,
    const std::vector<float>& radius) {
  auto collider       = add_collider(scene);
  collider->quads     = quads;
  collider->triangles = triangles;
  collider->positions = positions;
  collider->normals   = normals;
  collider->radius    = radius;
  return collider;
}

// Set shapes
void set_velocities(
    par::shape* shape, const vec3f& velocity, float random_scale) {
  shape->emit_velocity = velocity;
  shape->emit_rngscale = random_scale;
}

// Get shape properties
void get_positions(par::shape* shape, std::vector<vec3f>& positions) {
  positions = shape->positions;
}
void get_normals(par::shape* shape, std::vector<vec3f>& normals) {
  normals = shape->normals;
}

}  // namespace yocto::particle

// -----------------------------------------------------------------------------
// SIMULATION DATA AND API
// -----------------------------------------------------------------------------
namespace yocto::particle {

// Init simulation
void init_simulation(par::scene* scene, const simulation_params& params) {
    auto sid = 0;
    for (auto shape : scene->shapes) {
        shape->emit_rng   = make_rng(params.seed, (sid++) * 2 + 1);
        // YOUR CODE GOES HERE --------------------------------------------------
        // initialize state
        shape->positions = shape->initial_positions;
        shape->invmass = shape->initial_invmass;
        shape->velocities = shape->initial_velocities;
        shape->radius = shape->initial_radius;
        shape->normals = shape->initial_normals;
        shape->springs.clear();
        //shape->forces.clear();
        shape->forces = std::vector<vec3f>(shape->positions.size(), zero3f);
        
        // initialize pinned
        for(auto pinned : shape->initial_pinned)
            shape->invmass[pinned] = 0;
        
        // initialize velocities
        for(int i=0; i< shape->initial_velocities.size(); i++)
            shape->velocities[i] = yocto::math::sample_sphere(yocto::math::rand2f(shape->emit_rng)) * shape->emit_rngscale * rand1f(shape->emit_rng);
        
        // initialize springs
        // aggiungo le molle sui lati
        for(vec2i& edge: yocto::shape::get_edges(shape->quads))
        {
            if(shape->spring_coeff > 0)
            {
                auto& p1 = shape->positions[edge.x];
                auto& p2 = shape->positions[edge.y];
                shape->springs.push_back(spring{ edge.x, edge.y, yocto::math::distance(p1,p2), shape->spring_coeff});
            }
        }
        // aggiungo le molle sulle diagonali per non far piegare il vestito su se stesso
        for(vec4i q : shape->quads)
        {
            if(shape->spring_coeff > 0)
            {
                auto& p1 = shape->positions[q.x];
                auto& p2 = shape->positions[q.y];
                auto& p3 = shape->positions[q.z];
                auto& p4 = shape->positions[q.w];
                shape->springs.push_back(spring{q.x, q.z, yocto::math::distance(p1, p3), shape->spring_coeff});
                shape->springs.push_back(spring{q.y, q.w, yocto::math::distance(p2, p4), shape->spring_coeff});
            }
        }
    }
    for (auto collider : scene->colliders) {
        // YOUR CODE GOES HERE --------------------------------------------------
        // initialize bvh
        collider->bvh = {};
        if(collider->quads.size() > 0)
            yocto::shape::make_quads_bvh(collider->bvh, collider->quads, collider->positions, collider->radius);
        else
            yocto::shape::make_triangles_bvh(collider->bvh, collider->triangles, collider->positions, collider->radius);
    }
}

// check if a point is inside a collider
bool collide_collider(par::collider* collider, const vec3f& position,
    vec3f& hit_position, vec3f& hit_normal) {
    // YOUR CODE GOES HERE ----------------------------------------------------
    auto isec = yocto::shape::bvh_intersection{};
    //se sto trattando quad
    if(collider->quads.size()>0)
    {
        // intersezione (tirando un raggio nella direzione opposta alla direzione di gravità, cioè verso l'alto)
        isec = yocto::shape::intersect_quads_bvh(collider->bvh, collider->quads, collider->positions, ray3f{position, vec3f{0,1,0}});
        // se non c'è collisione --> false
        if(!isec.hit) return false;
        // altrimenti setto il punto in cui collide e la normale
        vec4i q = collider->quads[isec.element];
        hit_position = yocto::math::interpolate_quad(collider->positions[q.x], collider->positions[q.y], collider->positions[q.z], collider->positions[q.w], isec.uv);
        hit_normal = yocto::math::normalize(yocto::math::interpolate_quad(collider->normals[q.x], collider->normals[q.y], collider->normals[q.z], collider->normals[q.w], isec.uv));
    }
    // altrimenti sto trattando triangoli
    else
    {
        // intersezione (tirando un raggio nella direzione opposta alla direzione di gravità, cioè verso l'alto)
        isec = yocto::shape::intersect_triangles_bvh(collider->bvh, collider->triangles, collider->positions, ray3f{position, vec3f{0,1,0}});
        //se non c'è collisione --> false
        if(!isec.hit) return false;
        //altrimenti setto il punto in cui collide e la normale
        vec3i t = collider->triangles[isec.element];
        hit_position = yocto::math::interpolate_triangle(collider->positions[t.x], collider->positions[t.y], collider->positions[t.z], isec.uv);
        hit_normal = yocto::math::normalize(yocto::math::interpolate_triangle(collider->normals[t.x], collider->normals[t.y], collider->normals[t.z], isec.uv));
    }
    return dot(hit_normal, vec3f{0,1,0}) > 0;
}

// salva le vecchie posizioni
void save_old_pos(par::scene* scene) { for(auto& shape : scene->shapes) shape->old_positions = shape->positions; }

void adjust_vel(par::scene* scene, const simulation_params& params)
{
    for(auto& shape : scene->shapes)
    {
        for(int j=0; j<shape->positions.size(); j++)
        {
            if(shape->invmass[j] == 0) continue;
            // applico un velocity filter per il damping
            shape->velocities[j] *= (1.f - params.dumping * params.deltat);
            // se la velocità della mia particella è sotto una vel min --> fermo la particella
            if (length(shape->velocities[j]) < params.minvelocity) shape->velocities[j] = zero3f;
        }
    }
}


// simulate mass-spring
void simulate_massspring(par::scene* scene, const simulation_params& params) {
    // YOUR CODE GOES HERE ----------------------------------------------------
    
    // SAVE OLD POSITIONS
    save_old_pos(scene);

    
    // COMPUTE DYNAMICS
    for(int i=0; i<params.mssteps; i++)
    {
        auto ddt = params.deltat / params.mssteps;
        
        for(auto& shape : scene->shapes)
        {
            // compute forces
            for(int j=0; j<shape->positions.size(); j++)
            {
                if(shape->invmass[j] == 0) continue;
                shape->forces[j] = vec3f{0, -params.gravity, 0} / shape->invmass[j];
            }
            for(auto& spring : shape->springs)
            {
                // indici delle particle
                int particle0 = spring.vert0;
                int particle1 = spring.vert1;
                
                // se l'invmass è 0 sono pinned --> non devono spostarsi
                auto invmass = shape->invmass[particle0] + shape->invmass[particle1];
                if(invmass == 0) continue;
                
                // applico la formula per force
                auto delta_pos = shape->positions[particle1] - shape->positions[particle0];
                auto spring_dir = yocto::math::normalize(delta_pos);
                auto spring_len = yocto::math::length(delta_pos);
                auto delta_vel = shape->velocities[particle1] - shape->velocities[particle0];
                auto force = spring_dir * (spring_len / spring.rest - 1.f) / (invmass * spring.coeff);
                force += yocto::math::dot(delta_vel/spring.rest, spring_dir) * spring_dir / (1000.f * invmass * spring.coeff);
                shape->forces[particle0] += force;
                shape->forces[particle1] -= force;
            }
            //update state
            for(int j=0; j<shape->positions.size(); j++)
            {
                if(shape->invmass[j] == 0) continue;
                // aggiorno velocities e positions usando semi-implicit Euler
                shape->velocities[j] += ddt * shape->forces[j] * shape->invmass[j];
                shape->positions[j] += ddt * shape->velocities[j];
            }
            
        }
    }

    // HANDLE COLLISIONS
    for(auto& shape : scene->shapes)
    {
        for(int j=0; j<shape->positions.size(); j++)
        {
            // se è pinned passa avanti
            if(shape->invmass[j] == 0) continue;
            for(auto collider : scene->colliders)
            {
                auto hit_pos = zero3f;
                auto hit_normal = zero3f;
                // se c'è una collisione
                if(collide_collider(collider, shape->positions[j], hit_pos, hit_normal))
                {
                    // sposto la particella sul punto più "vicino" alla superficie ed applico un piccolo offset di 0.005
                    shape->positions[j] = hit_pos + hit_normal * 0.005f;
                    // applico un "rimbalzo" della particella
                    auto projection = dot(shape->velocities[j], hit_normal);
                    shape->velocities[j] = (shape->velocities[j] - projection * hit_normal) * (1.f - params.bounce.x) - projection * hit_normal * (1.f - params.bounce.y);
                }
            }
        }
    }
    // ADJUST VELOCITY
    adjust_vel(scene, params);
    
    // RECOMPUTE NORMALS
    for(auto& shape : scene->shapes)
    {
        if(shape->quads.size() > 0 )shape->normals = yocto::shape::compute_normals(shape->quads, shape->positions);
        else shape->normals = yocto::shape::compute_normals(shape->triangles, shape->positions);
    }
    
}

// simulate pbd
void simulate_pbd(par::scene* scene, const simulation_params& params) {
    // YOUR CODE GOES HERE ----------------------------------------------------

    // SAVE OLD POSITOINS
    save_old_pos(scene);

    // PREDICT POSITIONS
    for(auto& shape : scene->shapes)
    {
        for(int j=0; j<shape->positions.size(); j++)
        {
            if(shape->invmass[j] == 0) continue;
            // semi-implicit Euler su forze esterne
            shape->velocities[j] += vec3f{0, -params.gravity, 0} * params.deltat;
            shape->positions[j] += shape->velocities[j] * params.deltat;
        }
    }
    
    // DETECT COLLISIONS
    for(auto& shape: scene->shapes)
    {
        shape->collisions.clear();
        for(int j=0; j<shape->positions.size(); j++)
        {
            if(shape->invmass[j] == 0) continue;
            for(auto& collider : scene->colliders)
            {
                vec3f hit_pos = zero3f;
                vec3f hit_nor = zero3f;
                if(!collide_collider(collider, shape->positions[j], hit_pos, hit_nor)) continue;
                shape->collisions.push_back(collision{j, hit_pos, hit_nor});
            }
        }
    }

    // SOLVE CONSTRAINTS
    for(int i=0; i < params.pdbsteps; i++)
    {
        for(auto& shape : scene->shapes)
        {
            // gestione molle
            for(spring& spring : shape->springs)
            {
                int particle0 = spring.vert0;
                int particle1 = spring.vert1;
                float invmass = shape->invmass[particle0] + shape->invmass[particle1];
                if(invmass == 0) continue;
                vec3f dir = shape->positions[particle1] - shape->positions[particle0];
                float len = yocto::math::length(dir);
                dir /= len;
                auto lambda = (1.f - spring.coeff) * (len - spring.rest) / invmass;
                shape->positions[particle1] -= shape->invmass[particle1] * lambda * dir;
                shape->positions[particle0] += shape->invmass[particle0] * lambda * dir;
            }
            
            // gestione collisione
            for(auto& collision : shape->collisions)
            {
                int particle = collision.vert;
                if(shape->invmass[particle] == 0) continue;
                
                auto projection = dot(shape->positions[particle] - collision.position, collision.normal);
                if(projection >= 0) continue;
                // se la particella è entrata nell'oggetto la sposto
                shape->positions[particle] += -projection * collision.normal;
                
            }
        }
    }

    // COMPUTE VELOCITIES
    for(auto& shape : scene->shapes)
    {
        for(int j=0; j<shape->positions.size(); j++)
        {
            if(shape->invmass[j] == 0) continue;
            shape->velocities[j] = (shape->positions[j] - shape->old_positions[j]) / params.deltat;
        }
    }
    // ADJUST VELOCITY
    adjust_vel(scene, params);

    // RECOMPUTE NORMALS
    for(auto& shape : scene->shapes)
    {
        if(shape->quads.size() > 0 )shape->normals = yocto::shape::compute_normals(shape->quads, shape->positions);
        else shape->normals = yocto::shape::compute_normals(shape->triangles, shape->positions);
    }
}

// Simulate one step
void simulate_frame(par::scene* scene, const simulation_params& params) {
  switch (params.solver) {
    case solver_type::mass_spring: return simulate_massspring(scene, params);
    case solver_type::position_based: return simulate_pbd(scene, params);
    default: throw std::invalid_argument("unknown solver");
  }
}

// Simulate the whole sequence
void simulate_frames(par::scene* scene, const simulation_params& params,
    progress_callback progress_cb) {
  // handle progress
  auto progress = vec2i{0, 1 + (int)params.frames};

  if (progress_cb) progress_cb("init simulation", progress.x++, progress.y);
  init_simulation(scene, params);

  for (auto idx = 0; idx < params.frames; idx++) {
    if (progress_cb) progress_cb("simulate frames", progress.x++, progress.y);
    simulate_frame(scene, params);
  }

  if (progress_cb) progress_cb("simulate frames", progress.x++, progress.y);
}

}  // namespace yocto::particle
