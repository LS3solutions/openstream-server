function Component()
{
    var programFiles = installer.environmentVariable("ProgramFiles");
    if(programFiles != "")
        installer.setValue("TargetDir", programFiles + "\\Openstream");
}

Component.prototype.createOperations = function()
{
    try {
        component.createOperations();
        if(installer.value("os") == "win") {
            try {
                var userProfile = installer.environmentVariable("USERPROFILE");
                installer.setValue("UserProfile", userProfile);
                component.addOperation("CreateShortcut",
                                       "@TargetDir@\\openstreamapp.exe",
                                       "@UserProfile@\\Desktop\\Openstream.lnk",
                                       "workingDirectory=@TargetDir@",
                                       "iconPath=@TargetDir@\\openstreamapp.exe",
                                       "iconId=0",
                                       "description=Start Openstream server");
            } catch (e) {
                // Do Nothing
            }
        }
    } catch(e) {
        print(e);
    }

}
