# TrapJumpers
Music reactive LED Lamps with perfect diffusion

This project is heavily influenced by another maker who started me on this project, I will link his github below. There were problems with the code that ranged from signals dropping and I generally did not like how the master minion code was set up and I believe it could be improved so here we are.

Nerd Forge Project Github: https://github.com/hansjny/Natural-Nerd/tree/master/SoundReactive2

# Theory
using an analog Microphone for an arduino you can get a rough guess about the volume levels. Not perfect for music as the bass will heavily override and sound and sometimes vocals will get lost, however its stupid cheap and impresses people who don't pay that much attention, perfect for a party.
Below is a screenshot of what the analog sesnor reads on the serial plotter:
<img width="1161" alt="image" src="https://user-images.githubusercontent.com/64546287/131937734-52f60e93-6fb8-4416-88b4-49edb1bc4f39.png">
Each spike is a louder sound like a clap or in this case music segments. So when you have a filthy beat drop not only can you hear it in the music but you can see it in the lights. The baseline for this sensor is at around 550, I'm going to experement with subtracting that just directly so the math is easier but we will see I suppose.

# Formula
This is the formula that is used to translate the analog sensor value from the microphone to the corresponding LED height on the lamp 

<img width="424" alt="image" src="https://user-images.githubusercontent.com/64546287/132075862-bd7e10b4-a456-4b0c-8fd1-952e0d227ce0.png">

# Future Features
Adapative leveling: This would aggregate sensor values over a period of time like 10 minutes and constantly adapt the lower and upper limits to allow for better visualization if the music gets louder or quieter. 
~~Gradient Moving Rainbow Reaction: This would just simply move the gradient over time when in music reactive mode so there would be different colors at the peak and base level each mode switch~~ Live with latest update!
