__HITEC HS 35-HD DISCONTINUATION NOTICE:__ The Hitec servos used in this design have unfortunately been discontinued. We are currently testing other servo options. If you have built or would like to build a HANDi Hand, and would like information about pre-release parts for the new servo change design, please reach out to us at blincdev@gmail.com.

# HANDi Hand
The Humanoid, Anthropometric, Naturally Dexterous, Intelligent (HANDi) Hand is an anthropometric robotic hand designed specifically for machine learning research in prosthetic applications. The HANDi Hand includes 6 degrees of freedom (each finger flexes independently; thumb rotation is separate from thumb flexion). The hand was designed to be 1:1 scale with anatomical proportions and uses Hitec HS-35HD servo-motors. The HANDi Hand is ideally suited for machine learning and sensory feedback research, but can also be used in general purpose robotics applications.

## Build your own HANDi Hand
The HANDi Hand consists of off-the-shelf servomotors, potentiometers, FSRs, metric fasteners, and custom 3D printed parts. Instructions for building the hand and setting up the servos can be found in _'HANDi Hand - Assembly Manual.pdf'_. The assembly manual will be your main go-to document while printing and building the arm and includes references to the other files in the repository:


* __3D Print Files:__ The folder that contains the raw STL files as well as the .THING and .x3g files for 3D printing on a Makerbot Replicator 2. NOTE: The STL files have been adjusted to print with proper tolerances in the PLA material. Further, the print orientation is important for many of the files; refer to the .thing files provided if printing .STL files with other softwares besides MakerWare to ensure the proper print orientation.
* __STEP Files:__ This folder contains the STEP files for each of the parts used in the hand, which should be importable into most 3D CAD softwares.
* __Software:__ This folder contains Arduino sketches that are used for testing the hand during assembly, as well as control of the hand once assembly is complete.

The assembly manual also includes a Bill of Materials: a detailed list of the off-the-shelf materials and parts required to build the HANDi Hand. Note: We have listed the distributors that we have used in the past as a point of reference, but be aware that there are multiple distributor options for each part and that we do not endorse the listed distributors in any way.

## Contributing
You can share your experience, new design, ideas, feature requests, or questions on the [HANDi Hand project forum](https://blincdev.ca/forum/).

If you would like to contribute to future official releases of the HANDi Hand hardware we recommend contacting us on the forums to coordinate with our development team. To get started you will need to [fork this repo](https://help.github.com/articles/using-pull-requests/) and once your modification or enhancement is complete submit a [pull request](https://help.github.com/articles/using-pull-requests/).

## License
The HANDi Hand hardware is released under the [Creative Commons BY-SA 4.0](http://creativecommons.org/licenses/by-sa/4.0/) license. A local copy of the license that was forked from [https://github.com/idleberg/Creative-Commons-4.0-Markdown](https://github.com/idleberg/Creative-Commons-4.0-Markdown) is available in the repository. The HANDi Hand software is released under the [GNU General Public License v3](https://www.gnu.org/licenses/gpl.html). A local copy is available in the repository.
