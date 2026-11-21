# Sistema de Monitoramento de Ruído e Notificação Inteligente (ESP32 + MQTT)

Projeto da disciplina de **Microcontroladores**, do curso de **Automação Industrial (2º semestre)** do **IFCE – Campus Pecém**.

**Professor:** Tiago Façanha
**Integrantes:** Lucas Matias, Natan Estevão e Daniel Asafe

## Descrição

O ESP32 monitora o nível de ruído de um ambiente (como uma biblioteca). Quando o nível passa do limite definido, ele aciona os indicadores locais (LED e buzzer) e publica o status via MQTT, para que outros dispositivos possam receber o aviso.

## Hardware

| Componente | Pino do ESP32 | Função |
|---|---|---|
| Potenciômetro (simula o sensor de som) | GPIO 34 | Leitura analógica do nível de ruído |
| LED azul | GPIO 25 | Indica ruído **normal** |
| LED branco | GPIO 26 | Indica ruído **alto** |
| Buzzer | GPIO 27 | Aviso sonoro quando o ruído é alto |

Os LEDs usam resistores de 220 Ω.

## Como funciona

- O ESP32 lê o sinal analógico (12 bits, valores de 0 a 4095).
- Se o nível lido passa de **2000**, o ruído é considerado **alto**.
- Ruído normal: LED azul aceso e buzzer desligado.
- Ruído alto: LED branco aceso e buzzer tocando (`tone()`).
- O status é publicado a cada 3 segundos no tópico **`biblioteca/ruidoLAN`** do broker público **`broker.hivemq.com`** (porta 1883), em formato JSON:

```json
{"status":"normal","nivel":1577}
{"status":"alto","nivel":3627}
```

## Simulação

Projeto no Wokwi: https://wokwi.com/projects/480854124798968833

No simulador, o potenciômetro representa o sensor de som. Gire-o para aumentar ou diminuir o nível de ruído.

## Como testar o MQTT

1. Abra o cliente web do HiveMQ: https://www.hivemq.com/demos/websocket-client/
2. Clique em **Connect**.
3. Adicione uma assinatura (*Add New Topic Subscription*) no tópico `biblioteca/ruidoLAN`.
4. Inicie a simulação no Wokwi e gire o potenciômetro.
5. As mensagens com o status do ruído aparecem no cliente.

## Arquivos

- `sketch.ino`: código do ESP32
- `diagram.json`: circuito do Wokwi
- `libraries.txt`: bibliotecas usadas (PubSubClient)
