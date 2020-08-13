# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.ext_edtaa3func.Debug:
/Users/stianremvik/Development/Cocos2d-x/cocos2d-x-4.0_Games/BITWaves/proj.ios_mac/ios/lib/Debug/libext_edtaa3func.a:
	/bin/rm -f /Users/stianremvik/Development/Cocos2d-x/cocos2d-x-4.0_Games/BITWaves/proj.ios_mac/ios/lib/Debug/libext_edtaa3func.a


PostBuild.ext_edtaa3func.Release:
/Users/stianremvik/Development/Cocos2d-x/cocos2d-x-4.0_Games/BITWaves/proj.ios_mac/ios/lib/Release/libext_edtaa3func.a:
	/bin/rm -f /Users/stianremvik/Development/Cocos2d-x/cocos2d-x-4.0_Games/BITWaves/proj.ios_mac/ios/lib/Release/libext_edtaa3func.a


PostBuild.ext_edtaa3func.MinSizeRel:
/Users/stianremvik/Development/Cocos2d-x/cocos2d-x-4.0_Games/BITWaves/proj.ios_mac/ios/lib/MinSizeRel/libext_edtaa3func.a:
	/bin/rm -f /Users/stianremvik/Development/Cocos2d-x/cocos2d-x-4.0_Games/BITWaves/proj.ios_mac/ios/lib/MinSizeRel/libext_edtaa3func.a


PostBuild.ext_edtaa3func.RelWithDebInfo:
/Users/stianremvik/Development/Cocos2d-x/cocos2d-x-4.0_Games/BITWaves/proj.ios_mac/ios/lib/RelWithDebInfo/libext_edtaa3func.a:
	/bin/rm -f /Users/stianremvik/Development/Cocos2d-x/cocos2d-x-4.0_Games/BITWaves/proj.ios_mac/ios/lib/RelWithDebInfo/libext_edtaa3func.a




# For each target create a dummy ruleso the target does not have to exist
