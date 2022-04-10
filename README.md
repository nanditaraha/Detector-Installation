# Detector Testing and Installation
The MuSun experiment used eight liquid scintillator neutron detectors for detecting all neutrons, which were installed in four pairs forming an array
around the electron detector system in specific locations (labeled as 3, 6, 11 and 14) in the array of eSC (or gondolas). Thus, they were 
named NU3, ND3, NU6, ND6, NU11, ND11, NU14 and ND14, where ’U’ means upstream and means ’D’ downstream. They were placed in derlin holders and installed in 
their respective slots i.e. positions 3, 6 11 and 14 as shown in the left panel of the figure below. The neutron counters are essentially cylindrical cells
of 13 cm diameter and 13 cm depth and contain approximately 1.2 liters of BC501A organic liquid scintillator (which is a hydrocarbon called xylene) 
connected to a photo-multiplier tube (PMT). An example of this detector is shown in the right panel of the fig.</br> 
<img width="549" alt="Screen Shot 2022-04-10 at 10 14 00 AM" src="https://user-images.githubusercontent.com/27436642/162623551-e8b9a340-2633-446e-91ee-64799b0cee26.png"></br>
All this installation was done after testing the detectors, loading proper firmware and determinig the optimal voltage range for proper working. 
Online and offline event displays with this code tested all this. The code here checked the nuetron pulses. An example pulse is shown below:</br> 
<img width="298" alt="image" src="https://user-images.githubusercontent.com/27436642/162623518-5b7e0392-2592-488b-9ab0-e8dee58eb9a6.png"></br>
This code uses C++ and root trees.
