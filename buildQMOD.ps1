# Builds a .qmod file for loading with QuestPatcher
$NDKPath = Get-Content $PSScriptRoot/ndkpath.txt

$buildScript = "$NDKPath/build/ndk-build"
if (-not ($PSVersionTable.PSEdition -eq "Core")) {
    $buildScript += ".cmd"
}

$ArchiveName = "BrokenController_v1.0.3.qmod"
$TempArchiveName = "BrokenController_v1.0.3.qmod.zip"

& $buildScript NDK_PROJECT_PATH=$PSScriptRoot APP_BUILD_SCRIPT=$PSScriptRoot/Android.mk NDK_APPLICATION_MK=$PSScriptRoot/Application.mk -j 8

Compress-Archive -Path "./libs/arm64-v8a/libBrokenController.so", 
                       "./libs/arm64-v8a/libbeatsaber-hook_2_3_2.so", 
                       "./mod.json",
                       "./monkeyhammer.jpg" -DestinationPath $TempArchiveName -Force
Move-Item $TempArchiveName $ArchiveName -Force