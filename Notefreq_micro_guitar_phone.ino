/*
     C     C#    D     Eb    E     F     F#    G     G#    A     Bb    B
 0 16.35 17.32 18.35 19.45 20.60 21.83 23.12 24.50 25.96 27.50 29.14 30.87
 1 32.70 34.65 36.71 38.89 41.20 43.65 46.25 49.00 51.91 55.00 58.27 61.74
 2 65.41 69.30 73.42 77.78 82.41 87.31 92.50 98.00 103.8 110.0 116.5 123.5
 3 130.8 138.6 146.8 155.6 164.8 174.6 185.0 196.0 207.7 220.0 233.1 246.9
 4 261.6 277.2 293.7 311.1 329.6 349.2 370.0 392.0 415.3 440.0 466.2 493.9
 5 523.3 554.4 587.3 622.3 659.3 698.5 740.0 784.0 830.6 880.0 932.3 987.8
 6 1047  1109  1175  1245  1319  1397  1480  1568  1661  1760  1865  1976
 7 2093  2217  2349  2489  2637  2794  2960  3136  3322  3520  3729  3951
 8 4186  4435  4699  4978  5274  5588  5920  6272  6645  7040  7459  7902
 */
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputAnalog         adc1;           //xy=426,261 //A2
AudioAnalyzeNoteFrequency notefreq1;      //xy=686,82
AudioConnection          patchCord1(adc1, notefreq1);
// GUItool: end automatically generated code

// Déclaration des notes de référence 
//Guitar strings are E2=82.41Hz, A2=110Hz, D3=146.8Hz, G3=196Hz, B3=246.9Hz, E4=329.6Hz
const float guitar_notes[] = {82.41, 110, 146.8, 196, 246.9, 329.6};

//NOTES TEST TELEPHONE (A3 -> G3)
const float test_notes_3[] = {440, 493.88, 261.63, 293.66, 329,6, 349.2, 392};

//Intervale de validité sur la justesse de notre note (Hz)
const float intervalle_precision = 0.5;

int index_reference = 0; //stocke l'index du tableau dans lequel est associé la fréquence de référence
float note_ref_freq = 0.0; //valeur réelle de la fréquence de référence (pour être comparé à la fréquence échantillionée)
const char*  note_string; // String associé à la note de référence 
const int offset_phone = 250;
const int offset_guitar = 75;
int index_curseur = 0;

int mode = 1; //mode = 0 : test phone || mode = 1 : test guitar

void setup() {
    AudioMemory(100);
    Serial.begin(9600);
    notefreq1.begin(.15);
    pinMode(LED_BUILTIN, OUTPUT);

    if(mode==0)
      Serial.println(" - Mode téléphone -");
    else
      Serial.println(" - Mode guitare - ");

}

void loop() {
        
    // read back fundamental frequency 
    if (notefreq1.available()) {

      //Read results of Yin's algorithm 
      float note_sample = notefreq1.read();
      float prob_sample = notefreq1.probability();

      Serial.printf("Note: %3.2f | Probability: %.2f\n", note_sample, prob_sample);

      if(mode == 0){
        //Get the nearest reference index from the sample from phone
        index_reference = find_nearest_noteFreq_phone(note_sample);

        //Get frequency of the note at index_reference : 
        note_ref_freq = test_notes_3[index_reference];

        //Get the equivalent string from index_reference from phone
        note_string = find_noteString_from_index_phone(index_reference); // !!! VERSION PHONE
      }

      if(mode == 1){ //MODE GUITARE
        //Get the nearest reference index from the sample from guitar
        index_reference = find_nearest_noteFreq_guitar(note_sample); 
      
        //Get frequency of the note at index_reference : 
        note_ref_freq = guitar_notes[index_reference];
        
        //Get the equivalent string from index_reference from guitar
        note_string = find_noteString_from_index_guitar(index_reference); 
      }
      
    // ------------------------------------------- AFFICHAGE PHONE --------------------------------------------------------
      if(mode==0 && note_sample < 520){ //mode phone
        //Affichage curseur 
      
        index_curseur = (int)(note_sample - offset_phone)/1.2;
        for(int i = 0;i<index_curseur;i++)
          Serial.printf(" ");
        Serial.println("*");
        
        //Affichage axe
        for(int i=0;i<220;i++) {
            if(i==10 || i==36 || i==66 || i== 83 || i==118 || i==158 || i == 203)
              Serial.printf("|");
            else
              Serial.printf("-");
        }
        
        //Affichage note
        Serial.println();
        for(int i=0;i<220;i++) {
            switch(i){
              case 10 : 
                Serial.printf("C3");
                break;
              case 35 :
                Serial.printf("D3");
                break;
              case 64 : 
                Serial.printf("E3");
                break;
              case 80 : 
                Serial.printf("F3");
                break;
              case 114 : 
                Serial.printf("G3");
                break; 
             case 153 : 
                Serial.printf("A3");
                break; 
             case 197 : 
                Serial.printf("B3");
                break; 
             default : 
                Serial.printf(" ");
                break; 
            }
        }
              
        Serial.println();
        Serial.println();
      }

    //------------------------------------------- AFFICHAGE GUITARE --------------------------------------------------------
    
      if(mode==1){ //MODE GUITARE
        //Affichage curseur 
      
        index_curseur = (int)(note_sample - offset_guitar)/1.2;
        for(int i = 0;i<index_curseur;i++)
          Serial.printf(" ");
        Serial.println("*");
        
        //Affichage axe
        for(int i=0;i<221;i++)
            if(i==6 || i==42 || i==60 || i== 101 || i==143 || i==212)
              Serial.printf("|");
            else
              Serial.printf("-");
                      
        //Affichage note
        Serial.println();
        for(int i=0;i<220;i++) {
            switch(i){
              case 6 : 
                Serial.printf("E2");
                break;
              case 41 :
                Serial.printf("A2");
                break;
              case 58 : 
                Serial.printf("D3");
                break;
              case 98 : 
                Serial.printf("G3");
                break; 
             case 139 : 
                Serial.printf("B3");
                break; 
             case 207 : 
                Serial.printf("E4");
                break; 
             default : 
                Serial.printf(" ");
                break; 
            }
        }
        Serial.println();
        Serial.println();
      }
         
    }
     
}

int find_nearest_noteFreq_phone(float note_freq){
  
  float dist_from_ref[7];
  float dist_minimal = 0.0;
  int index_minimal = 0;

  // Calculate all distante between the sample and all reference note
  dist_from_ref[0] = abs(note_freq - test_notes_3[0]);
  dist_from_ref[1] = abs(note_freq - test_notes_3[1]);
  dist_from_ref[2] = abs(note_freq - test_notes_3[2]);
  dist_from_ref[3] = abs(note_freq - test_notes_3[3]);
  dist_from_ref[4] = abs(note_freq - test_notes_3[4]);
  dist_from_ref[5] = abs(note_freq - test_notes_3[5]);
  dist_from_ref[6] = abs(note_freq - test_notes_3[6]);

  //get minimum distance frequency in the array
  dist_minimal = getMin(dist_from_ref, 7);

  //find index of dist_minimal
  int i=0;
  for(i = 0;i<7;i++){ 
      if(dist_from_ref[i] == dist_minimal){ 
        index_minimal = i;
        break;
        }
  }
  return index_minimal;
}

int find_nearest_noteFreq_guitar(float note_freq){
  
  float dist_from_ref[6];
  float dist_minimal = 0.0;
  int index_minimal = 0;

  // Calculate all distante between the sample and all reference note
  dist_from_ref[0] = abs(note_freq - guitar_notes[0]);
  dist_from_ref[1] = abs(note_freq - guitar_notes[1]);
  dist_from_ref[2] = abs(note_freq - guitar_notes[2]);
  dist_from_ref[3] = abs(note_freq - guitar_notes[3]);
  dist_from_ref[4] = abs(note_freq - guitar_notes[4]);
  dist_from_ref[5] = abs(note_freq - guitar_notes[5]);
  
  //get minimum distance frequency in the array
  dist_minimal = getMin(dist_from_ref, 6);

  //find index of dist_minimal
  int i=0;
  for(i = 0;i<6;i++){ 
      if(dist_from_ref[i] == dist_minimal){ 
        index_minimal = i;
        break;
        }
  }
  return index_minimal;
}

const char * find_noteString_from_index_phone(int index_note){

  const char* chord_str;
  //Convert this index_minimal in note 
  switch (index_note) {
    case 0:
      chord_str = "A3";
      break;
    case 1:
      chord_str = "B3";
      break;
    case 2:
      chord_str = "C3";
      break;
    case 3 : 
      chord_str = "D3";
      break;
    case 4 : 
      chord_str = "E3";
      break;
    case 5 : 
      chord_str = "F3";
      break;
    case 6 : 
      chord_str = "G3";
      break;
    default:
      chord_str = "NO";
      break;
  }

  return chord_str;
}

const char * find_noteString_from_index_guitar(int index_note){

  const char* chord_str;
  //Convert this index_minimal in note 
  switch (index_note) {
    case 0:
      chord_str = "E2";
      break;
    case 1:
      chord_str = "A2";
      break;
    case 2:
      chord_str = "D3";
      break;
    case 3 : 
      chord_str = "G3";
      break;
    case 4 : 
      chord_str = "E3";
      break;
    case 5 : 
      chord_str = "E4";
      break;
    default:
      chord_str = "NO";
      break;
  }

  return chord_str;
}

float getMin(float * array_, int size_array){
  float minimum = array_[0]; //une distance
  
  for(int i = 1;i<size_array;i++){
    if(array_[i] < minimum) minimum = array_[i];  
  }
  return minimum;
}
