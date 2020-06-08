# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.glfw.Debug:
/Users/eduardo/Desktop/out/bin/Debug/libglfw3.a:
	/bin/rm -f /Users/eduardo/Desktop/out/bin/Debug/libglfw3.a


PostBuild.yocto.Debug:
/Users/eduardo/Desktop/out/bin/Debug/libyocto.a:
	/bin/rm -f /Users/eduardo/Desktop/out/bin/Debug/libyocto.a


PostBuild.yocto_gui.Debug:
/Users/eduardo/Desktop/out/bin/Debug/libyocto_gui.a:
	/bin/rm -f /Users/eduardo/Desktop/out/bin/Debug/libyocto_gui.a


PostBuild.yocto_particle.Debug:
/Users/eduardo/Desktop/out/bin/Debug/libyocto_particle.a:
	/bin/rm -f /Users/eduardo/Desktop/out/bin/Debug/libyocto_particle.a


PostBuild.yparticletrace.Debug:
PostBuild.yocto.Debug: /Users/eduardo/Desktop/out/bin/Debug/yparticletrace
PostBuild.yocto_particle.Debug: /Users/eduardo/Desktop/out/bin/Debug/yparticletrace
PostBuild.yocto.Debug: /Users/eduardo/Desktop/out/bin/Debug/yparticletrace
/Users/eduardo/Desktop/out/bin/Debug/yparticletrace:\
	/Users/eduardo/Desktop/out/bin/Debug/libyocto.a\
	/Users/eduardo/Desktop/out/bin/Debug/libyocto_particle.a\
	/Users/eduardo/Desktop/out/bin/Debug/libyocto.a
	/bin/rm -f /Users/eduardo/Desktop/out/bin/Debug/yparticletrace


PostBuild.yparticleviews.Debug:
PostBuild.yocto.Debug: /Users/eduardo/Desktop/out/bin/Debug/yparticleviews
PostBuild.yocto_gui.Debug: /Users/eduardo/Desktop/out/bin/Debug/yparticleviews
PostBuild.yocto_particle.Debug: /Users/eduardo/Desktop/out/bin/Debug/yparticleviews
PostBuild.glfw.Debug: /Users/eduardo/Desktop/out/bin/Debug/yparticleviews
PostBuild.yocto.Debug: /Users/eduardo/Desktop/out/bin/Debug/yparticleviews
/Users/eduardo/Desktop/out/bin/Debug/yparticleviews:\
	/Users/eduardo/Desktop/out/bin/Debug/libyocto.a\
	/Users/eduardo/Desktop/out/bin/Debug/libyocto_gui.a\
	/Users/eduardo/Desktop/out/bin/Debug/libyocto_particle.a\
	/Users/eduardo/Desktop/out/bin/Debug/libglfw3.a\
	/Users/eduardo/Desktop/out/bin/Debug/libyocto.a
	/bin/rm -f /Users/eduardo/Desktop/out/bin/Debug/yparticleviews


PostBuild.glfw.Release:
/Users/eduardo/Desktop/out/bin/Release/libglfw3.a:
	/bin/rm -f /Users/eduardo/Desktop/out/bin/Release/libglfw3.a


PostBuild.yocto.Release:
/Users/eduardo/Desktop/out/bin/Release/libyocto.a:
	/bin/rm -f /Users/eduardo/Desktop/out/bin/Release/libyocto.a


PostBuild.yocto_gui.Release:
/Users/eduardo/Desktop/out/bin/Release/libyocto_gui.a:
	/bin/rm -f /Users/eduardo/Desktop/out/bin/Release/libyocto_gui.a


PostBuild.yocto_particle.Release:
/Users/eduardo/Desktop/out/bin/Release/libyocto_particle.a:
	/bin/rm -f /Users/eduardo/Desktop/out/bin/Release/libyocto_particle.a


PostBuild.yparticletrace.Release:
PostBuild.yocto.Release: /Users/eduardo/Desktop/out/bin/Release/yparticletrace
PostBuild.yocto_particle.Release: /Users/eduardo/Desktop/out/bin/Release/yparticletrace
PostBuild.yocto.Release: /Users/eduardo/Desktop/out/bin/Release/yparticletrace
/Users/eduardo/Desktop/out/bin/Release/yparticletrace:\
	/Users/eduardo/Desktop/out/bin/Release/libyocto.a\
	/Users/eduardo/Desktop/out/bin/Release/libyocto_particle.a\
	/Users/eduardo/Desktop/out/bin/Release/libyocto.a
	/bin/rm -f /Users/eduardo/Desktop/out/bin/Release/yparticletrace


PostBuild.yparticleviews.Release:
PostBuild.yocto.Release: /Users/eduardo/Desktop/out/bin/Release/yparticleviews
PostBuild.yocto_gui.Release: /Users/eduardo/Desktop/out/bin/Release/yparticleviews
PostBuild.yocto_particle.Release: /Users/eduardo/Desktop/out/bin/Release/yparticleviews
PostBuild.glfw.Release: /Users/eduardo/Desktop/out/bin/Release/yparticleviews
PostBuild.yocto.Release: /Users/eduardo/Desktop/out/bin/Release/yparticleviews
/Users/eduardo/Desktop/out/bin/Release/yparticleviews:\
	/Users/eduardo/Desktop/out/bin/Release/libyocto.a\
	/Users/eduardo/Desktop/out/bin/Release/libyocto_gui.a\
	/Users/eduardo/Desktop/out/bin/Release/libyocto_particle.a\
	/Users/eduardo/Desktop/out/bin/Release/libglfw3.a\
	/Users/eduardo/Desktop/out/bin/Release/libyocto.a
	/bin/rm -f /Users/eduardo/Desktop/out/bin/Release/yparticleviews


PostBuild.glfw.MinSizeRel:
/Users/eduardo/Desktop/out/bin/MinSizeRel/libglfw3.a:
	/bin/rm -f /Users/eduardo/Desktop/out/bin/MinSizeRel/libglfw3.a


PostBuild.yocto.MinSizeRel:
/Users/eduardo/Desktop/out/bin/MinSizeRel/libyocto.a:
	/bin/rm -f /Users/eduardo/Desktop/out/bin/MinSizeRel/libyocto.a


PostBuild.yocto_gui.MinSizeRel:
/Users/eduardo/Desktop/out/bin/MinSizeRel/libyocto_gui.a:
	/bin/rm -f /Users/eduardo/Desktop/out/bin/MinSizeRel/libyocto_gui.a


PostBuild.yocto_particle.MinSizeRel:
/Users/eduardo/Desktop/out/bin/MinSizeRel/libyocto_particle.a:
	/bin/rm -f /Users/eduardo/Desktop/out/bin/MinSizeRel/libyocto_particle.a


PostBuild.yparticletrace.MinSizeRel:
PostBuild.yocto.MinSizeRel: /Users/eduardo/Desktop/out/bin/MinSizeRel/yparticletrace
PostBuild.yocto_particle.MinSizeRel: /Users/eduardo/Desktop/out/bin/MinSizeRel/yparticletrace
PostBuild.yocto.MinSizeRel: /Users/eduardo/Desktop/out/bin/MinSizeRel/yparticletrace
/Users/eduardo/Desktop/out/bin/MinSizeRel/yparticletrace:\
	/Users/eduardo/Desktop/out/bin/MinSizeRel/libyocto.a\
	/Users/eduardo/Desktop/out/bin/MinSizeRel/libyocto_particle.a\
	/Users/eduardo/Desktop/out/bin/MinSizeRel/libyocto.a
	/bin/rm -f /Users/eduardo/Desktop/out/bin/MinSizeRel/yparticletrace


PostBuild.yparticleviews.MinSizeRel:
PostBuild.yocto.MinSizeRel: /Users/eduardo/Desktop/out/bin/MinSizeRel/yparticleviews
PostBuild.yocto_gui.MinSizeRel: /Users/eduardo/Desktop/out/bin/MinSizeRel/yparticleviews
PostBuild.yocto_particle.MinSizeRel: /Users/eduardo/Desktop/out/bin/MinSizeRel/yparticleviews
PostBuild.glfw.MinSizeRel: /Users/eduardo/Desktop/out/bin/MinSizeRel/yparticleviews
PostBuild.yocto.MinSizeRel: /Users/eduardo/Desktop/out/bin/MinSizeRel/yparticleviews
/Users/eduardo/Desktop/out/bin/MinSizeRel/yparticleviews:\
	/Users/eduardo/Desktop/out/bin/MinSizeRel/libyocto.a\
	/Users/eduardo/Desktop/out/bin/MinSizeRel/libyocto_gui.a\
	/Users/eduardo/Desktop/out/bin/MinSizeRel/libyocto_particle.a\
	/Users/eduardo/Desktop/out/bin/MinSizeRel/libglfw3.a\
	/Users/eduardo/Desktop/out/bin/MinSizeRel/libyocto.a
	/bin/rm -f /Users/eduardo/Desktop/out/bin/MinSizeRel/yparticleviews


PostBuild.glfw.RelWithDebInfo:
/Users/eduardo/Desktop/out/bin/RelWithDebInfo/libglfw3.a:
	/bin/rm -f /Users/eduardo/Desktop/out/bin/RelWithDebInfo/libglfw3.a


PostBuild.yocto.RelWithDebInfo:
/Users/eduardo/Desktop/out/bin/RelWithDebInfo/libyocto.a:
	/bin/rm -f /Users/eduardo/Desktop/out/bin/RelWithDebInfo/libyocto.a


PostBuild.yocto_gui.RelWithDebInfo:
/Users/eduardo/Desktop/out/bin/RelWithDebInfo/libyocto_gui.a:
	/bin/rm -f /Users/eduardo/Desktop/out/bin/RelWithDebInfo/libyocto_gui.a


PostBuild.yocto_particle.RelWithDebInfo:
/Users/eduardo/Desktop/out/bin/RelWithDebInfo/libyocto_particle.a:
	/bin/rm -f /Users/eduardo/Desktop/out/bin/RelWithDebInfo/libyocto_particle.a


PostBuild.yparticletrace.RelWithDebInfo:
PostBuild.yocto.RelWithDebInfo: /Users/eduardo/Desktop/out/bin/RelWithDebInfo/yparticletrace
PostBuild.yocto_particle.RelWithDebInfo: /Users/eduardo/Desktop/out/bin/RelWithDebInfo/yparticletrace
PostBuild.yocto.RelWithDebInfo: /Users/eduardo/Desktop/out/bin/RelWithDebInfo/yparticletrace
/Users/eduardo/Desktop/out/bin/RelWithDebInfo/yparticletrace:\
	/Users/eduardo/Desktop/out/bin/RelWithDebInfo/libyocto.a\
	/Users/eduardo/Desktop/out/bin/RelWithDebInfo/libyocto_particle.a\
	/Users/eduardo/Desktop/out/bin/RelWithDebInfo/libyocto.a
	/bin/rm -f /Users/eduardo/Desktop/out/bin/RelWithDebInfo/yparticletrace


PostBuild.yparticleviews.RelWithDebInfo:
PostBuild.yocto.RelWithDebInfo: /Users/eduardo/Desktop/out/bin/RelWithDebInfo/yparticleviews
PostBuild.yocto_gui.RelWithDebInfo: /Users/eduardo/Desktop/out/bin/RelWithDebInfo/yparticleviews
PostBuild.yocto_particle.RelWithDebInfo: /Users/eduardo/Desktop/out/bin/RelWithDebInfo/yparticleviews
PostBuild.glfw.RelWithDebInfo: /Users/eduardo/Desktop/out/bin/RelWithDebInfo/yparticleviews
PostBuild.yocto.RelWithDebInfo: /Users/eduardo/Desktop/out/bin/RelWithDebInfo/yparticleviews
/Users/eduardo/Desktop/out/bin/RelWithDebInfo/yparticleviews:\
	/Users/eduardo/Desktop/out/bin/RelWithDebInfo/libyocto.a\
	/Users/eduardo/Desktop/out/bin/RelWithDebInfo/libyocto_gui.a\
	/Users/eduardo/Desktop/out/bin/RelWithDebInfo/libyocto_particle.a\
	/Users/eduardo/Desktop/out/bin/RelWithDebInfo/libglfw3.a\
	/Users/eduardo/Desktop/out/bin/RelWithDebInfo/libyocto.a
	/bin/rm -f /Users/eduardo/Desktop/out/bin/RelWithDebInfo/yparticleviews




# For each target create a dummy ruleso the target does not have to exist
/Users/eduardo/Desktop/out/bin/Debug/libglfw3.a:
/Users/eduardo/Desktop/out/bin/Debug/libyocto.a:
/Users/eduardo/Desktop/out/bin/Debug/libyocto_gui.a:
/Users/eduardo/Desktop/out/bin/Debug/libyocto_particle.a:
/Users/eduardo/Desktop/out/bin/MinSizeRel/libglfw3.a:
/Users/eduardo/Desktop/out/bin/MinSizeRel/libyocto.a:
/Users/eduardo/Desktop/out/bin/MinSizeRel/libyocto_gui.a:
/Users/eduardo/Desktop/out/bin/MinSizeRel/libyocto_particle.a:
/Users/eduardo/Desktop/out/bin/RelWithDebInfo/libglfw3.a:
/Users/eduardo/Desktop/out/bin/RelWithDebInfo/libyocto.a:
/Users/eduardo/Desktop/out/bin/RelWithDebInfo/libyocto_gui.a:
/Users/eduardo/Desktop/out/bin/RelWithDebInfo/libyocto_particle.a:
/Users/eduardo/Desktop/out/bin/Release/libglfw3.a:
/Users/eduardo/Desktop/out/bin/Release/libyocto.a:
/Users/eduardo/Desktop/out/bin/Release/libyocto_gui.a:
/Users/eduardo/Desktop/out/bin/Release/libyocto_particle.a:
