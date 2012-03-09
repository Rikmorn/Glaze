#Glaze

__Description__

Fairly old project I made for my masters. 
It's a visualization framework for mobile devices that is based on the information visualization reference model. It uses a plugin architecture to allow for dynamic visualization setup. 
A paper detailing this can be found [here](http://www.whereveriam.org/work/UMa/Sousa-Nisi-Oakley-Glaze.pdf).

The project already has a couple of visualizations used as a test case:

* First is what we called Night Vision. This visualization uses, besides the phone, a GPS and a digital compass. The GPS allows the visualization  to  track  the user's position  in  the real world while the digital compass tracks where the user is looking at. By mapping position and orientation to a virtual equivalent we can replicate what the user sees in the real world with a virtual replica, thus allowing them to navigate in a low lighting condition. As for the model the more detailed it is the more it will look like its real counterpart and allow for easily identifiable landmarks. 
* Second is the information overlay visualization is the classic example to realize all the requirements asked for the project. It consists of overlaying on top of the camera feed information about what we are currently seeing, from general knowledge, related events, geographical information to comments from other people that have also seen it.  The objective of this visualization is to present relevant or interesting information about what is around the user. There are obvious uses for tourism as a way of exploration and discovery of foreign places. This visualization uses the same hardware as the night vision allowing for complete component reuse without any change within them.  

__Instalation__

This project was developed in a windows system using Visual Studio (2005 I remember correcly) for the Nokia N95.
As long as the directory structure present in this folder is maitained and the SDK is properly installed there shouldn't be any problems compiling.
The target phone must have SyExpat dll instaled for Glaze to run. Simply install the provided sisx file (expatinstall.sisx) on the phone and everything should be fine.
Developed and tested in Carbide.c++ 2.0.

This code has been discontinued, but if you are interested in it feel free to use it.
