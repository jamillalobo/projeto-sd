#include <LiquidCrystal_I2C.h>

// Inicializa o LCD com o endereço I2C 0x27 (pode variar, verifique o seu módulo)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Definindo os pinos
const int ledTurno = 3;
const int ledEspaco = 5;
const int ledVibe = 8;

const int botaoProximoTurno = 2;
const int botaoProximoEspaco = 4;
const int botaoProximaVibe = 7;
const int botaoConfirmar = 9;

// Declaração do estado
enum Estado {ESCOLHENDO_TURNO, ESCOLHENDO_ESPACO, ESCOLHENDO_VIBE, FINALIZADO};
Estado estadoAtual = ESCOLHENDO_TURNO;

// Contador dos loops
int turno = 0;
int espaco = 0;
int vibe = 0;

bool turnoConfirmado = false;
bool espacoConfirmado = false;
bool vibeConfirmado = false;

const char* turnos[] = {"Manha", "Tarde", "Noite"};
const char* espacos[] = {"Fechado", "Ar livre"};
const char* vibes[] = {"Instagramavel", "Romantica", "Divertida"};

// Definição do struct Local
struct Local {
  const char* turno;
  const char* espaco;
  const char* vibe;
  const char* nomeLocal;
};

// Array para armazenar todas as combinações possíveis
Local locais[18] = {
  {"Manha", "Fechado", "Instagramavel", "Paco Frevo"},
  {"Manha", "Ar livre", "Instagramavel", "Parque Gracas"},
  {"Manha", "Fechado", "Romantica", "Vida eh Bela"},
  {"Manha", "Ar livre", "Romantica", "Picnic Federal"},
  {"Manha", "Fechado", "Divertida", "MAMAM Recife"},
  {"Manha", "Ar livre", "Divertida", "Pedalar Jaqueira"},
  {"Tarde", "Fechado", "Instagramavel", "Livraria Praca"},
  {"Tarde", "Ar livre", "Instagramavel", "Jardim Botanico"},
  {"Tarde", "Fechado", "Romantica", "Cafe com Dengo"},
  {"Tarde", "Ar livre", "Romantica", "Picnic Jaqueira"},
  {"Tarde", "Fechado", "Divertida", "Vila Trampolim"},
  {"Tarde", "Ar livre", "Divertida", "Castelo Brennand"},
  {"Noite", "Fechado", "Instagramavel", "Yokocho Izakaya"},
  {"Noite", "Ar livre", "Instagramavel", "Mercado Torre"},
  {"Noite", "Fechado", "Romantica", "Pina Cocktails"},
  {"Noite", "Ar livre", "Romantica", "Barco Rio Capyba"},
  {"Noite", "Fechado", "Divertida", "Rockstar Pub"},
  {"Noite", "Ar livre", "Divertida", "Rua da Moeda"}
};

void setup() {
  // Inicializa o display I2C
  lcd.init();
  lcd.begin(16, 2);
  lcd.backlight();

  // Mostra uma mensagem no display
  lcd.setCursor(0, 0);
  lcd.print("Date Perfeito");
  delay(2000);
  
  pinMode(ledTurno, OUTPUT);
  pinMode(ledEspaco, OUTPUT);
  pinMode(ledVibe, OUTPUT);
  
  pinMode(botaoProximoTurno, INPUT_PULLUP); 
  pinMode(botaoProximoEspaco, INPUT_PULLUP);  
  pinMode(botaoProximaVibe, INPUT_PULLUP);
  pinMode(botaoConfirmar, INPUT_PULLUP);  
}

void loop() {
  switch (estadoAtual) {
    case ESCOLHENDO_TURNO:
      digitalWrite(ledTurno, HIGH);
      lcd.setCursor(0, 0);
      lcd.print("Escolha o Turno:");
      lcd.setCursor(0, 1);
      lcd.print(turnos[turno]);
    
      if (!turnoConfirmado) {
        if (digitalRead(botaoProximoTurno) == LOW) {
          turno++;
          if (turno > 2){
            turno = 0;
          }
          delay(200);
          lcd.clear(); 
        }
        if (digitalRead(botaoConfirmar) == LOW){
          turnoConfirmado = true;
          estadoAtual = ESCOLHENDO_ESPACO;
          delay(200);
          lcd.clear(); 
        }
      } 
      break;

    case ESCOLHENDO_ESPACO:
      digitalWrite(ledTurno, LOW);
      digitalWrite(ledEspaco, HIGH);
      lcd.setCursor(0, 0);
      lcd.print("Escolha o Espaco:");
      lcd.setCursor(0, 1);
      lcd.print(espacos[espaco]);

      if (!espacoConfirmado) {
        if (digitalRead(botaoProximoEspaco) == LOW) {
          espaco++;
          if (espaco > 1){
            espaco = 0;
          }
          delay(200);
          lcd.clear(); 
        }
        if (digitalRead(botaoConfirmar) == LOW){
          espacoConfirmado = true;
          estadoAtual = ESCOLHENDO_VIBE;
          delay(200);
          lcd.clear(); 
        }
      } 
      break;

    case ESCOLHENDO_VIBE:
      digitalWrite(ledEspaco, LOW);
      digitalWrite(ledVibe, HIGH);
      lcd.setCursor(0, 0);
      lcd.print("Escolha a Vibe:");
      lcd.setCursor(0, 1);
      lcd.print(vibes[vibe]);

      if (!vibeConfirmado) {
        if (digitalRead(botaoProximaVibe) == LOW) {
          vibe++;
          if (vibe > 2){
            vibe = 0;
          }
          delay(200);
          lcd.clear();
        }
        if (digitalRead(botaoConfirmar) == LOW){
          vibeConfirmado = true;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Escolhendo...");
          estadoAtual = FINALIZADO;
          delay(500);
          lcd.clear(); 
        }
      } 
      break;

    case FINALIZADO:
      digitalWrite(ledVibe, LOW);
      lcd.clear();
      
      // Encontrar o local correspondente
      Local localSelecionado;
      for (int i = 0; i < 18; ++i) {
        if (locais[i].turno == turnos[turno] && locais[i].espaco == espacos[espaco] && locais[i].vibe == vibes[vibe]) {
          localSelecionado = locais[i];
          break;
        }
      }

      // Exibir o local selecionado
      lcd.setCursor(0, 0);
      lcd.print("Local:");
      lcd.setCursor(0, 1);
      lcd.print(localSelecionado.nomeLocal);
      delay(6000);

      // Resetar endpoints e começar novamente
      turnoConfirmado = false;
      espacoConfirmado = false;
      vibeConfirmado = false;
      turno = 0;
      espaco = 0;
      vibe = 0;
      estadoAtual = ESCOLHENDO_TURNO;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Date Perfeito");
      delay(2000);
      break;
  }
}
