# monitor-ruido-esp32-mqtt
# Sistema de Monitoramento de Ruído com ESP32 e MQTT

Projeto que monitora o nível de ruído e publica o status via MQTT.

## Hardware
- ESP32
- Sensor de som (simulado com potenciômetro no pino 34)
- LED verde (pino 25), LED vermelho (pino 26)
- Buzzer (pino 27)

## Como funciona
- Se o nível lido passa de 2000, o ruído é considerado alto.
- LED vermelho e buzzer são acionados localmente.
- O status é publicado a cada 3 s no tópico `biblioteca/ruido`
  do broker `broker.hivemq.com`.

## Simulação
https://wokwi.com/projects/480854124798968833

## Como testar o MQTT
Assinar o tópico `biblioteca/ruido` no cliente web do HiveMQ:
https://www.hivemq.com/demos/websocket-client/
