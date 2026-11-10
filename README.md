# Carrinho IOT

Primeira etapa do painel: header e joystick, em HTML, CSS e JavaScript puro.
Abra `index.html` ou sirva a pasta com um servidor HTTP estático.

O modo DEMO está desligado. O joystick envia comandos HTTP ao carrinho em `192.168.10.103`. Arraste com mouse ou toque; solte para parar. Com o joystick em foco, as setas do teclado também funcionam.

O protocolo é `GET /comando?acao=...`, com ações `frente`, `re`, `direita`, `esquerda` e `parar`, igual ao controle físico. O indicador verifica `GET /` a cada 5 segundos; o firmware precisa responder com HTTP 200 nessa rota para a verificação funcionar.

O celular ou PC precisa estar na mesma rede Wi-Fi do carrinho. A página não conecta o aparelho ao Wi-Fi. Não inclua a senha da rede no frontend.

Prefira servir os arquivos deste projeto pelo próprio ESP32 e acessar `http://192.168.10.103/`. Nesse endereço, o JavaScript usa a mesma origem. Servir os arquivos exige implementação no firmware, que não está neste repositório. Se usar outro servidor, configure CORS no firmware. O HTTPS do GitHub Pages pode bloquear acesso ao IP HTTP, e o navegador pode exigir permissão de rede local. Use hospedagem no ESP32 para evitar a mistura HTTPS/HTTP, sem desativar proteções do navegador.

A sintaxe e o envio das cinco ações foram verificados com respostas simuladas; a conexão e os motores ainda precisam ser testados com o hardware.

Esta etapa não implementa leitura de telemetria nem controle físico de velocidade. Antes do uso real, implemente um watchdog no firmware para parar os motores quando os comandos deixarem de chegar: o navegador não garante a entrega de `/parar` se a rede cair ou a página for encerrada.

As fontes Orbitron e Rajdhani vêm do Google Fonts, com fallback local. Todos os elementos gráficos são CSS e SVG embutido; não há dependências JavaScript.

## Orientação e tela cheia

O painel funciona nas duas orientações. Na vertical, a área de informações fica acima do joystick. O tamanho acompanha a altura disponível e as áreas seguras da tela.

O botão no canto superior direito alterna tela cheia nos navegadores compatíveis. Em celulares, o primeiro toque fora do joystick também tenta ativá-la; não há nova tentativa automática depois de sair. O navegador exige interação do usuário para essa API. No Safari/iPhone, use Compartilhar → Adicionar à Tela de Início e abra pelo ícone para usar o modo standalone. O suporte varia conforme navegador e sistema; o manifesto não implementa funcionamento offline.
