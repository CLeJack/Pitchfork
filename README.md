Pitchfork is a project derived from Scribe, the audio to MIDI converter. Instead of producing midi, it plots the frequency spectrum and the harmonic spectrum of an incoming signal.

### Source
---

#### Plugin Generation

**PluginProcessor** - Handles all audio processing. It is mostly unmodified from the default JUCE file, and wraps Pitchfork

**DataModels** - these models are exposed globally to allow communication between the Editor (GUI) and AudioProcessors
- Pitchfork - a struct that stores all of the processed data used to generate plots and system data that is used to make internal calculations
- AudioParams - a struct that stores user adjustable parameters allowing the behavior of Pitchfork to be modified

**ModelInstances** - creates Pitchfork and AudioParams in a global scope.

**PluginEditor** - Handles the gui processing and pulls data from the Pitchfork struct to present it to the user.

**GUIX** - various customized GUI components that inherit from JUCE built in objects
---

#### Calculations and processing

**CircularBuffer** - receives a continuous stream of data that can be converted to a vector when needed

**DCT** - Discrete Customized (Fourier) Transform. 
- Allows specific frequencies to be analyzed. 
- removes constraints of window size and frequency resolution
- loses the ability to reconstruct a signal or get absolute amplitude information

**Head** - defines some convenience macro definitions and a few inline functions. Included at the top of other header files

**Stats** - some basic statistic functions

**Tuning** - Contains the math necessary for Equal Temperament tuning, and other concepts associated with western music theory

**Waveforms** - sin, complex sin, and time vectors necessary to calculate waveforms

**ProcessData** - a hold over from Scribe, this may be integrated into Stats later.
---

#### Testing

**/tests** - Conatins files for visually testing output data (as .csv) on .wav files without building the full plugin.
 - This is decoupled from the PluginEditor and PluginProcessor.




