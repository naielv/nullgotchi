// Test of making sound using PWM capable pin and a piezo.

const int OUTPUT_PIN = 3; // must be pwm capable

typedef uint32_t frequency_t;
typedef unsigned int note_number_t;

// A table of 1024 * 2^x * 440 for x in 1/12 increments from x=0 to x=11/12.
// These are used to convert MIDI numbers to frequencies. Since
//
//  f = 2^((note-69)/12) * 440 Hz
const frequency_t NOTE_TABLE[] = {
  450560, 477351, 505736, 535809, 567670, 601425, 637188, 675077, 715219,
  757748, 802806, 850544
};

inline frequency_t midi_to_frequency(note_number_t note) {
  int shift_note = (int)note - 69;
  frequency_t f = NOTE_TABLE[shift_note % 12];

  // Compute shift required taking into account shift by 10 built into
  // NOTE_TABLE.
  int freq_shift = shift_note/12 - 10;

  if(freq_shift < 0) {
    return f >> (-freq_shift);
  }

  return f << freq_shift;
}

void setup() {
  pinMode(OUTPUT_PIN, OUTPUT);
}

const note_number_t notes[] = {
  72, 74, 76, 77, 79, 81, 83, 84,
};

const int n_notes = sizeof(notes) / sizeof(notes[0]);

void loop() {
  for(int transpose=0; transpose<=12; ++transpose) {
    for(int i=0; i<n_notes; ++i) {
      tone(OUTPUT_PIN, midi_to_frequency(transpose + notes[i]));
      delay(60000 / 240);
    }
    noTone(OUTPUT_PIN);

    delay(500);

    for(int i=0; i<n_notes; ++i) {
      tone(OUTPUT_PIN, midi_to_frequency(transpose + notes[n_notes - i - 1]));
      delay(60000 / 240);
    }
    noTone(OUTPUT_PIN);

    delay(500);
  }
}
