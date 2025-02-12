// Definição dos pinos
#define TRIG_PIN 9
#define ECHO_PIN 8
#define IR_SENSOR A0
#define ENA 3
#define ENB 11
#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7

// Parâmetros
#define LIMIAR_IR 500
#define DISTANCIA_OBJETO 60 //(cm)
#define TEMPO_ALINHAMENTO 300

// Declaração antecipada das funções
void moverFrente();
void girar();
void parar();
void moverTras();

// Função para medir distância 
long medirDistancia() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    return pulseIn(ECHO_PIN, HIGH) * 0.034 / 2;
}

// Função para verificar fim da arena
bool fimDaArena() {
    return analogRead(IR_SENSOR) < LIMIAR_IR;
}

// Controles de movimento (ordem corrigida)
void moverFrente() {
    analogWrite(ENA, 130);
    analogWrite(ENB, 130);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}

void moverTras() {
    analogWrite(ENA, 130);
    analogWrite(ENB, 130);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

void girar() {
    analogWrite(ENA, 130);
    analogWrite(ENB, 150);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}

void parar() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}

// Função voltarCentro 
void voltarCentro() {
    // Recuo inicial
    moverTras();
    unsigned long inicioRecuo = millis();
    while (fimDaArena() && (millis() - inicioRecuo < 1000)) {
        delay(10);
    }
    parar();
    
    //  Alinhamento
    analogWrite(ENA, 100);
    analogWrite(ENB, 100);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    delay(TEMPO_ALINHAMENTO);
    parar();
    
    // Retorno
    moverFrente();
    unsigned long inicioAvancando = millis();
    while (millis() - inicioAvancando < 800) {
        if (fimDaArena()) {
            parar();
            return;
        }
        delay(10);
    }
    parar();
}

void setup() {
    Serial.begin(9600);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(IR_SENSOR, INPUT);
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
}

void loop() {
    // Busca por objetos
    while (medirDistancia() > DISTANCIA_OBJETO) {
        girar();
        if (fimDaArena()) {
            voltarCentro();
        }
    }

    // Aproximação 
    unsigned long inicioAproximacao = millis();
    while (medirDistancia() > 5 && (millis() - inicioAproximacao < 2000)) {
        moverFrente();
        if (fimDaArena()) {
            voltarCentro();
            return;
        }
    }
    parar();

    // Movimento final
    while (!fimDaArena()) {
        moverFrente();
        delay(50);
    }
    
    voltarCentro();
}
