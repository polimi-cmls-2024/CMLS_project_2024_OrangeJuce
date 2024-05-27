// Avvia il server audio
(
s.boot;
s.options.sampleRate = 48000;
)

//Main code
(
NetAddr("127.0.0.1",57120);
~osc = NetAddr("127.0.0.1",15000);
~processing = NetAddr("127.0.0.1", 9000);
~charArray = [ ];
~note = Array.newClear(50);
~time = 1;
~speed = 0.1;
~capacity = 0;
~detune_sequencer = 0;
~selectedWave = 1;
~amplitude_value = 0.5;

~generateWave = {
	arg frequency, waveType, detune, phase, octave, link_lfo, lfo_freq, lfo_depth;
	var osc, lfo = SinOsc.kr(lfo_freq, 0, lfo_depth, frequency);
	osc = Select.ar(link_lfo, [
		Select.ar(waveType, [
			SinOsc.ar(frequency * (2 ** octave) * (1 + detune), phase),
			Saw.ar(frequency * (2 ** octave) * (1 + detune), 1, phase),
			Pulse.ar(frequency * (2 ** octave) * (1 + detune), 0.5, 1, phase)
		]) ,
	    Select.ar(waveType, [
			SinOsc.ar(lfo * (2 ** octave), phase),
			Saw.ar(lfo * (2 ** octave), 1,  phase),
			Pulse.ar(lfo * (2 ** octave), 0.5,1, phase)])]);
	osc;
};

~mixSignal = {
	arg button_status, osc_1, osc_2, osc_3;
	var sig;
	sig = Select.ar(button_status, [
		osc_1, osc_2, osc_3, osc_1+osc_2, osc_1+osc_3, osc_2+osc_3, osc_1+osc_2+osc_3
	]);
	sig;
};

SynthDef(\mySynth, {
    arg freq = 440, amp = 0, pan = 0, cutoff = 20000, button_status=0, resonance = 0.5,
	phase_1 = 0, phase_2 = 0, phase_3 = 0,
	attack_1 = 0.1, attack_2 = 0.1, attack_3 = 0.1,
	release_1 = 1, release_2 = 1, release_3 = 1,
	sustain_1 = 0.5, sustain_2 = 0.5, sustain_3 = 0.5,
	detune_1 = 0, detune_2 = 0, detune_3 = 0,
	waveType_1 = 0, waveType_2 = 0, waveType_3 = 0,
	lfo_depth_1 = 10, lfo_depth_2 = 0.5, lfo_depth_3 = 0.5,
	lfo_freq_1 = 6, lfo_freq_2 = 6, lfo_freq_3 = 6,
	link_lfo_1 = 0, link_lfo_2 = 0, link_lfo_3 = 0,
	octave_1 = 0, octave_2 = 0, octave_3 = 0;
	var env_1, env_2, env_3, signal, lfo_1, lfo_2, lfo_3, osc_1, osc_2, osc_3;
	osc_1 = ~generateWave.value(freq, waveType_1, detune_1, phase_1, octave_1, link_lfo_1, lfo_freq_1, lfo_depth_1); //waveform
	osc_2 = ~generateWave.value(freq, waveType_2, detune_2, phase_2, octave_2, link_lfo_2, lfo_freq_2, lfo_depth_2);
	osc_3 = ~generateWave.value(freq, waveType_3, detune_3, phase_3, octave_3, link_lfo_3, lfo_freq_3, lfo_depth_3);
	env_1 = EnvGen.kr(Env.asr(attack_1, sustain_1, release_1), doneAction: 2); //envelope
	env_2 = EnvGen.kr(Env.asr(attack_2, sustain_2, release_2), doneAction: 2);
	env_3 = EnvGen.kr(Env.asr(attack_3, sustain_3, release_3), doneAction: 2);
	osc_1 = osc_1 * env_1; //link the waveform to envelope
	osc_2 = osc_2 * env_2;
	osc_3 = osc_3 * env_3;
	signal = ~mixSignal.value(button_status, osc_1, osc_2, osc_3); //mix the signals
	signal = signal * amp;
    signal = Pan2.ar(signal, pan); //panning
    signal = RLPF.ar(signal, cutoff, resonance); //cutoff
	Out.ar(0, signal); //output

}).add;

~x = Synth(\mySynth);

~sequencer = Routine({
	var index = 0;
	loop {
		~x.set(\amp, ~amplitude_value);
		if (~detune_sequencer == 0, {~x.set(\freq, ~note[index])} , {~x.set(\freq, ~note[index] * (2 ** (~detune_sequencer/12)))});
		if (index == (~capacity-1)) {~speed.wait; ~x.set(\amp, 0); ~time.wait};
		if (index != (~capacity-1)) {~speed.wait};
		index = (index + 1) % ~capacity;
	}
});

~map = {
	arg perc, min, max;
	var output;
	output = min + (perc/100) * (max - min);
	output;
};

~readSerialValue = {
	arg input;
	var number = (input/1000).asInteger, value = (input%1000).asInteger;
	if (number == 1) {
		if (~selectedWave == 1) {~x.set(\phase_1, ~map.value(value, 0, 360))};
		if (~selectedWave == 2) {~x.set(\phase_2, ~map.value(value, 0, 360))};
		if (~selectedWave == 3) {~x.set(\phase_3, ~map.value(value, 0, 360))};
		postln("Phase " + ~selectedWave + ": " + ~map.value(value, 0, 360));
		~processing.sendMsg("/setPhase", ~selectedWave.asInteger, ~map.value(value, 0, 360).asFloat);
		~osc.sendMsg("/changePhase", ~selectedWave.asInteger, ~map.value(value, 0, 360).asFloat);
	};
	if (number == 2) {
		if (~selectedWave == 1) {~x.set(\detune_1, ((value/50)-1))};
		if (~selectedWave == 2) {~x.set(\detune_2, ((value/50)-1))};
		if (~selectedWave == 3) {~x.set(\detune_3, ((value/50)-1))};
		postln("Detune " + ~selectedWave + ": " + ((value/50)-1));
		~osc.sendMsg("/changeDetune", ~selectedWave.asInteger, ((value/50)-1).asFloat);
	};

	if (number == 3) {
		~speed = ~map.value(value, 0, 0.5);
		postln("Speed: " + ~map.value(value, 0, 0.5));
		~osc.sendMsg("/changeSpeed", ~map.value(value, 0, 0.5).asFloat);
		~processing.sendMsg("/setSpeed", ~map.value(value, 0, 500).asInteger);
	};

	if (number == 4) {
		~time = ~map.value(value, 0, 0.5);
		postln("Time: " + ~map.value(value, 0, 0.5));
		~osc.sendMsg("/changeTime", ~map.value(value, 0, 0.5).asFloat);
		~processing.sendMsg("/setTime", ~map.value(value, 0, 500).asInteger);
	};

	if (number == 5) {
		~detune_sequencer = (value-50);
		postln("Detune Sequencer: " + (value-50));
		~osc.sendMsg("/changeDetune_seq", (value-50).asFloat);
	};

	if (number == 6) {
		if (~selectedWave == 1) {~x.set(\lfo_freq_1, ~map.value(value, 0,20))};
		if (~selectedWave == 2) {~x.set(\lfo_freq_2, ~map.value(value, 0,20))};
		if (~selectedWave == 3) {~x.set(\lfo_freq_3, ~map.value(value, 0,20))};
		postln("Lfo Frequency " + ~selectedWave + ": " + ~map.value(value, 0,20));
		~osc.sendMsg("/changeFreq", ~selectedWave.asInteger, ~map.value(value, 0,20).asFloat);
	};

	if (number == 7) {
		if (~selectedWave == 1) {~x.set(\lfo_depth_1, ~map.value(value, 0,100))};
		if (~selectedWave == 2) {~x.set(\lfo_depth_2, ~map.value(value, 0,100))};
		if (~selectedWave == 3) {~x.set(\lfo_depth_3, ~map.value(value, 0,100))};
		postln("Lfo Depth " + ~selectedWave + ": " + value);
		~osc.sendMsg("/changeDepth", ~selectedWave.asInteger, ~map.value(value, 0,100).asFloat);
	};
	if (number == 8) {
		~selectedWave = 1;
		postln("selected wave 1");
	};
	if (number == 9) {
		if ((input/100).asInteger == 95, { ~selectedWave = 3; postln("selected wave 3");} , {
			~selectedWave = 2;
			postln("selected wave 2");});
	};
};

OSCdef('OSCreceiver_frequency', //Osc receiver for the note one
	{
	arg msg;
	var frequency = msg[1];
	~x.set(\freq, frequency);
	~x.set(\amp, ~amplitude_value);
		postln("frequency: "+ frequency);
}, '/setFrequency');

OSCdef('OSCreceiver_stop', //Osc receiver for the note off
	{
	arg msg;
	~x.set(\amp, 0);
		postln("Note Stopped" + msg[1]);
}, '/stopNote');

OSCdef('OSCreceiver_attack', //Osc receiver for the attack
	{
	arg msg;
	var selection = msg[1], value = msg[2];
	if (selection == 0) {~x.set(\attack_1, value)};
	if (selection == 1) {~x.set(\attack_2, value)};
	if (selection == 2) {~x.set(\attack_3, value)};
	postln("Attack " + selection +": "+ value);
}, '/setAttack');

OSCdef('OSCreceiver_phase', //Osc receiver for the attack
	{
	arg msg;
	var selection = msg[1], value = msg[2];
	if (selection == 0) {~x.set(\phase_1, value)};
	if (selection == 1) {~x.set(\phase_2, value)};
	if (selection == 2) {~x.set(\phase_3, value)};
	postln("Phase " + selection +": "+ value);
}, '/setPhase');

OSCdef('OSCreceiver_release', //Osc receiver for the release
	{
	arg msg;
	var selection = msg[1], value = msg[2];
	postln("Release " + selection +": "+ value);
}, '/setRelease');

OSCdef('OSCreceiver_sustain', //Osc receiver for the sustain
	{
	arg msg;
	var selection = msg[1], value = msg[2];
	postln("Sustain " + selection +": "+ value);
}, '/setSustain');

OSCdef('OSCreceiver_wavetype', //Osc receiver for the wavetype
	{
	arg msg;
	var selection = msg[1], value = msg[2];
	if (selection == 0) {~x.set(\waveType_1, value)};
	if (selection == 1) {~x.set(\waveType_2, value)};
	if (selection == 2) {~x.set(\waveType_3, value)};
	postln("Wavetype " + selection +": "+ value);
}, '/setWavetype');

OSCdef('OSCreceiver_detune', //Osc receiver for the detune
	{
	arg msg;
	var selection = msg[1], value = msg[2];
	if (selection == 0) {~x.set(\detune_1, value)};
	if (selection == 1) {~x.set(\detune_2, value)};
	if (selection == 2) {~x.set(\detune_3, value)};
	postln("Detune " + selection +": "+ value);
}, '/setDetune');

OSCdef('OSCreceiver_button', //Osc receiver for the button status
	{
	arg msg;
	var value = msg[1];
	~x.set(\button_status, value);
	postln("Button status: " + value);
}, '/setButton');

OSCdef('OSCreceiver_cutoff', //Osc receiver for the cutoff
	{
	arg msg;
	var value = msg[1];
	~x.set(\cutoff, value);
	postln("Cutoff: " + value);
}, '/setCutoff');

OSCdef('OSCreceiver_resonance.', //Osc receiver for the resonance
	{
	arg msg;
	var value = msg[1];
	~x.set(\resonance, value);
	postln("Resonance: " + value);
}, '/setResonance');

OSCdef('OSCreceiver_octave', //Osc receiver for the octave
	{
	arg msg;
	var selection = msg[1], value = msg[2];
	if (selection == 0) {~x.set(\octave_1, value)};
	if (selection == 1) {~x.set(\octave_2, value)};
	if (selection == 2) {~x.set(\octave_3, value)};
	postln("Octave " + selection +": "+ value);
}, '/setOctave');

OSCdef('OSCreceiver_pan', //Osc receiver for panning
	{
	arg msg;
	var value = msg[1];
	~x.set(\pan, value);
	postln("Pan: " + value);
}, '/setPan');

OSCdef('OSCreceive_sequencer', //Osc receiver for the sequencer and sequencer code
{
		arg msg;
		~capacity = msg[1].asInteger;
		~capacity.do{
			arg x;
			~note[x] = msg[x+2];
		};
		postln("Sequencer quantity: " + ~capacity);
}, '/playSequencer');

OSCdef('OSCreceiver_time', //Osc receiver for time in sequencer
	{
	arg msg;
	var value = msg[1];
	~time = value;
	postln("Time: " + value);
}, '/setTime');

OSCdef('OSCreceiver_amplitude', //Osc receiver for amplitude
	{
	arg msg;
	var value = msg[1];
	~amplitude_value = value;
	postln("Amplitude: " + value);
}, '/setAmplitude');

OSCdef('OSCreceiver_speed', //Osc receiver for speed in sequencer
	{
	arg msg;
	var value = msg[1];
	~speed = value;
	postln("Speed: " + value);
}, '/setSpeed');

OSCdef('OSCreceiver_freq', //Osc receiver for lfo depth
	{
	arg msg;
	var selection = msg[1], value = msg[2];
	if (selection == 0) {~x.set(\lfo_freq_1, value)};
	if (selection == 1) {~x.set(\lfo_freq_2, value)};
	if (selection == 2) {~x.set(\lfo_freq_3, value)};
	postln("LFO Frequency " + selection +": "+ value);
}, '/setFreq_lfo');

OSCdef('OSCreceiver_depth', //Osc receiver for lfo depth
	{
	arg msg;
	var selection = msg[1], value = msg[2];
	if (selection == 0) {~x.set(\lfo_depth_1, value)};
	if (selection == 1) {~x.set(\lfo_depth_2, value)};
	if (selection == 2) {~x.set(\lfo_depth_3, value)};
	postln("LFO Depth " + selection +": "+ value);
}, '/setDepth');

OSCdef('OSCreceiver_play_sequencer', //Osc receiver to start the sequencer
	{
	~sequencer.reset;
	~sequencer.play;
		postln("play");
}, '/play');

OSCdef('OSCreceiver_stop_sequencer', //Osc receiver fto stop the sequencer
	{
	~sequencer.stop;
	~x.set(\amp, 0);
	postln("stop");
}, '/stop');

OSCdef('OSCreceiver_detune_sequencer', //Osc receiver fto stop the sequencer
	{
	arg msg;
	var value = msg[1];
	~detune_sequencer = value;
	postln("Detune_sequencer: " + value);
}, '/setDetuneSequencer');

OSCdef('OSCreceiver_lfo_button', //Osc receiver for lfo button change
	{
	arg msg;
	var selection = msg[1], value = msg[2].asInteger;
	if (selection == 1) {~x.set(\link_lfo_1, value)};
	if (selection == 2) {~x.set(\link_lfo_2, value)};
	if (selection == 3) {~x.set(\link_lfo_3, value)};
	postln("LFO button " + selection +": "+ value);
}, '/setLFOButton');
)

//Arduino code serial reading
(
~port = SerialPort.new("/dev/cu.usbmodem2101", 9600);
~getValues = Routine.new({
	var ascii;
	{
		ascii = ~port.read.asAscii;
		if (ascii.isDecDigit, {~charArray = ~charArray.add(ascii)});
		if (ascii == $a, {
			~val = ~charArray.collect(_.digit).convertDigits;
			~charArray = [];
			~readSerialValue.value(~val);
		});
	}.loop;
}).play;

)

SerialPort.devices;

// Termina il synth
x.free;