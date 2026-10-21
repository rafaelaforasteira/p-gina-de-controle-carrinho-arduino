# Carrinho IOT

Primeira etapa do painel: header e joystick, em HTML, CSS e JavaScript puro.
Abra `index.html` ou sirva a pasta com um servidor HTTP estático.

O modo DEMO está ligado. Conexão e IP são ilustrativos. O console registra os comandos do joystick. Arraste com mouse ou toque; solte para parar. Com o joystick em foco, as setas do teclado também funcionam.

Para integrar posteriormente, altere `DEMO_MODE` em `app.js` para `false`. As rotas são `/frente`, `/re`, `/direita`, `/esquerda` e `/parar`. `ESP32_BASE_URL` vazio usa a origem da página; se outro servidor for usado, configure o endereço do ESP32 e CORS no firmware. Uma página HTTPS também pode bloquear acesso HTTP ao ESP32.

Esta etapa não implementa leitura de telemetria nem controle físico de velocidade. Antes do uso real, implemente um watchdog no firmware para parar os motores quando os comandos deixarem de chegar: o navegador não garante a entrega de `/parar` se a rede cair ou a página for encerrada.

As fontes Orbitron e Rajdhani vêm do Google Fonts, com fallback local. Todos os elementos gráficos são CSS e SVG embutido; não há dependências JavaScript.

## Orientação e tela cheia

O painel funciona nas duas orientações. Na vertical, a área de informações fica acima do joystick. O tamanho acompanha a altura disponível e as áreas seguras da tela.

O botão no canto superior direito alterna tela cheia nos navegadores compatíveis. Em celulares, o primeiro toque fora do joystick também tenta ativá-la; não há nova tentativa automática depois de sair. O navegador exige interação do usuário para essa API. No Safari/iPhone, use Compartilhar → Adicionar à Tela de Início e abra pelo ícone para usar o modo standalone. O suporte varia conforme navegador e sistema; o manifesto não implementa funcionamento offline.
