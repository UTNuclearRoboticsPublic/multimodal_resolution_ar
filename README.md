# multimodal_resolution_ar

This repository contains the code and resources for the demonstration of Large Language Model (LLM) driven task planning for Embodied AI (EAI) systems, using Augmented Reality AR headsets (HoloLens 2) for human-robot teaming.
The multi grounding technique combines voice commands and interactive interfaces via Virtual Markers.

## Requirements: 

* Unreal Engine 4.27
* [Robofleet Server](https://github.com/UTNuclearRobotics/robofleet.git) on your local network.
  * System that enables ROS-based robot-to-robot communication
* [Azure Spatial Anchor Account](https://azure.microsoft.com/en-us/services/spatial-anchors/)
  * Built in Microsoft feature to allows HoloLens and Robots co-localize with each other.
* [gpt_umrf_parser node](https://github.com/temoto-framework/gpt_umrf_parser/tree/demo_test) 


## Usage
1. Clone the Repo
 ```sh
 git clone --recursive https://github.com/UTNuclearRobotics/multimodal_resolution_ar.git
 ```    
Update the Submodules.
```sh
git submodule update --init --recursive
```
2. Generate the solution
3. Package the project and deploy to the HoloLens Device


Find mode documentation on how to run and deploy apps using Unreal engine [here](https://learn.microsoft.com/en-us/windows/mixed-reality/develop/unreal/unreal-quickstart)

# Run the Demo
* Start the app on the HoloLens
* Open the TeMoto interface 
  * You can use the hand menu and tap on top of the TeMoto Icon, or use the "TeMoto" voice command to open the chat-like interface
A coordinate system and a chat window will appear. Place the coordinate frame at a desire location, for this you can use near or far interactios using hand gestures.

* Use the chat-style interface to send a command. You can type the request or send a voice command using the microphone from the keyboard i.e. "robot inspect that area".

The HoloLens uses visual marker information and voice commands to generate a prompt in a string format, which is then sent to the gpt_parser_node through the `/command` topic. The node appends a few examples to the prompt and sends the request to the GPT model. It takes some time for the model to generate an output, and once it is ready, a sequence of blocks is spawned on the HoloLens, representing the UMRF Graph as feedback, with a combination of `navigation`, `manipulation`, and `take_photo` actions. The robot should start executing the graph. Each block is highlighted with a different color based on the state of execution.

