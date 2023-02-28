# AugREImagePlugin
This is a plugin for unreal engine to have video stremed from ROS image_raw/compressed topics


Note: This plugin will not work without RobofleetUnrealClient (https://github.com/UTNuclearRoboticsPublic/RobofleetUnrealClient) integrated as a module in your project.


How to bring the plugin into your Unreal project:
1. Make a Plugin folder (If your Unreal project already has a plugin folder ignore):
    Go into the folder explorer location of your project and create a folder titled "Plugins" in the same directory/level as your "Source" folder. Open this folder after it is created. 
2. Git clone this plugin into the "Plugins" folder you just made. 
3. Change the .uproject file of your entreito include the plugin. To do this, nagigate to the directory with your source folder and open the <project name>.uproject file. Inside if you already have a plugin you will see a "Plugins" section most likley uner the "Modules" section which will include at least your project name. If there is no "Plugins" section, you will need to add it in the format below. (where you see "snazzy_project" you should see your project name)
    
![image](https://user-images.githubusercontent.com/55891096/167676933-6c1764dd-ef9f-4cd4-a0bb-42931b6ae299.png)
    
4. Now you should be able to run the project however the plugin does not currently connect with the robotfleet server so the plugin will currently not play a streamed video until that is rectified. 
5. Note: it is posible that the first time you run the project you will receive errors with the BP_Dynamic_canvas blue print. This can be fixed by the folowing steps:
        
    1. In world outliner, click "edit BP_VideoPLayer"
    
    2. Under functions, open Initialize Dynamic Canvas
    
    3. In blueprints, right-click, make a new blueprint, under classes search for dynamic canvas, name this blueprint BP_DynamicCanvas
    
    4. Change the dynamic canvas object to a texture2dObjectReference with a Get Dynamic Canvas block
    
  pictures will be added soon^
