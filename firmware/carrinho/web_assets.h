#pragma once
#include <Arduino.h>

// Gerado a partir dos arquivos da raiz. Execute python3 scripts/embed_web.py.

const char INDEX_HTML[] PROGMEM = R"CARRINHO(<!doctype html>
<html lang="pt-BR">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width,initial-scale=1,viewport-fit=cover">
  <meta name="theme-color" content="#050505">
  <meta name="mobile-web-app-capable" content="yes">
  <meta name="apple-mobile-web-app-capable" content="yes">
  <meta name="apple-mobile-web-app-status-bar-style" content="black-translucent">
  <meta name="apple-mobile-web-app-title" content="Carrinho IOT">
  <link rel="manifest" href="manifest.webmanifest">
  <title>Carrinho IOT — Controle</title>
  <link rel="preconnect" href="https://fonts.googleapis.com">
  <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
  <link href="https://fonts.googleapis.com/css2?family=Orbitron:wght@600;700;800;900&family=Rajdhani:wght@400;500;600;700&display=swap" rel="stylesheet">
  <link rel="stylesheet" href="styles.css">
  <script src="app.js" defer></script>
</head>
<body>
  <main class="cockpit">
    <header class="header">
      <div class="connection disconnected" aria-label="Desconectado — ESP32">
        <span class="connection-dot"></span>
        <div class="connection-copy"><strong>DESCONECTADO</strong><span>192.168.10.103</span></div>
      </div>
      <div class="brand"><svg class="brand-trace" viewBox="0 0 650 80" preserveAspectRatio="none" aria-hidden="true"><path d="M0 18H105Q120 18 130 28L158 51Q165 57 182 57H468Q485 57 492 51L520 28Q530 18 545 18H650"/></svg><div class="brand-plate"><h1>CARRINHO <span>IOT</span></h1></div></div>
      <button class="fullscreen-button" id="fullscreen-button" type="button" aria-label="Entrar em tela cheia" title="Entrar em tela cheia" aria-pressed="false">
        <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.6" stroke-linecap="round" stroke-linejoin="round" aria-hidden="true"><path class="expand-icon" d="M8 3H3v5m13-5h5v5M3 16v5h5m13-5v5h-5"/><path class="collapse-icon" d="M3 8h5V3m8 0v5h5M8 21v-5H3m18 0h-5v5"/></svg>
      </button>
    </header>
    <div id="fullscreen-hint" class="fullscreen-hint" role="status" hidden></div>
    <section class="deck" aria-label="Painel de controle">
      <div class="control-bay">
        <div class="joystick" id="joystick" role="application" tabindex="0" aria-label="Joystick. Arraste para dirigir e solte para parar. Use também as setas do teclado." data-direction="parar">
          <div class="dial-ticks"></div><div class="outer-rim"></div><div class="light-arcs"></div><div class="inner-rim"></div><div class="carbon-disc"></div>
          <span class="direction-label front" data-highlight="frente">FRENTE</span><span class="direction-label rear" data-highlight="re">RÉ</span><span class="direction-label left" data-highlight="esquerda">ESQUERDA</span><span class="direction-label right" data-highlight="direita">DIREITA</span>
          <svg class="arrow arrow-front" data-highlight="frente" viewBox="0 0 40 26" aria-hidden="true"><path d="M3 21 20 4l17 17-5 1L20 10 8 22z"/></svg>
          <svg class="arrow arrow-rear" data-highlight="re" viewBox="0 0 40 26" aria-hidden="true"><path d="M3 21 20 4l17 17-5 1L20 10 8 22z"/></svg>
          <svg class="arrow arrow-left" data-highlight="esquerda" viewBox="0 0 40 26" aria-hidden="true"><path d="M3 21 20 4l17 17-5 1L20 10 8 22z"/></svg>
          <svg class="arrow arrow-right" data-highlight="direita" viewBox="0 0 40 26" aria-hidden="true"><path d="M3 21 20 4l17 17-5 1L20 10 8 22z"/></svg>
          <div class="knob-socket"></div><div class="knob" id="knob"><div></div></div>
        </div>
        <div class="movement-plate"><i></i><output id="movement" aria-live="polite">PARADO</output><i></i></div>
      </div>
      <div class="reserved-panel" aria-hidden="true"><div class="floor-grid"></div><div class="lower-trace"></div></div>
    </section>
    <footer class="edge-details"><span>ESP32 / CONTROLE</span></footer>
  </main>
</body>
</html>
)CARRINHO";

const char STYLES_CSS[] PROGMEM = R"CARRINHO(:root{color-scheme:dark;--red:#ff2525;--text:#dbdbdd;font-family:Rajdhani,'Arial Narrow',sans-serif;background:#050505;color:var(--text)}
*{box-sizing:border-box}html,body{width:100%;height:100%;margin:0;overflow:hidden;overscroll-behavior:none}body{height:100dvh;user-select:none;-webkit-user-select:none;background:radial-gradient(ellipse at 22% 100%,#31050555,transparent 48%),#050505;padding:clamp(8px,2vw,28px);padding-left:max(clamp(8px,2vw,28px),env(safe-area-inset-left));padding-right:max(clamp(8px,2vw,28px),env(safe-area-inset-right))}
.cockpit{height:100%;position:relative;isolation:isolate;overflow:hidden;border:1px solid #373738;border-radius:clamp(18px,3vw,42px);background:radial-gradient(ellipse at 22% 40%,#22222540,transparent 46%),linear-gradient(115deg,#0c0c0e,#030304 65%,#0b0b0c);box-shadow:0 0 0 3px #080809,0 0 0 4px #414143,inset 0 2px 2px #6b6b6b55,inset 0 -3px 4px #000,0 15px 45px #000;display:flex;flex-direction:column;padding:0 clamp(16px,3vw,45px)}
.cockpit:before{content:'';pointer-events:none;position:absolute;inset:0;z-index:-1;opacity:.2;background:repeating-linear-gradient(0deg,transparent 0 3px,#ffffff03 3px 4px),repeating-linear-gradient(90deg,transparent 0 89px,#73737315 89px 90px)}
.header{height:clamp(58px,12vh,100px);flex-shrink:0;display:flex;align-items:center;justify-content:space-between;position:relative;border-bottom:1px solid #30303366;gap:12px}.header:after{content:'';position:absolute;bottom:-2px;left:0;right:0;height:1px;background:#000}
.connection{display:flex;gap:clamp(6px,1vw,14px);align-items:center;z-index:2}.connection-dot{height:clamp(12px,1.65vw,22px);aspect-ratio:1;border-radius:50%;background:#06db2d;box-shadow:inset 0 1px 2px #89ff92,0 0 13px #00ed2630;animation:connected 4s infinite}.connection-copy{display:flex;flex-direction:column;line-height:1.12}.connection-copy strong{font-size:clamp(12px,1.4vw,21px);font-weight:700}.connection-copy>span{font-size:clamp(11px,1.3vw,19px);color:#7d7d80;margin-top:3px}.wifi{width:clamp(23px,2.6vw,38px);margin-left:clamp(3px,1.4vw,22px)}.signal{display:flex;gap:3px;height:24px;align-items:flex-end}.signal i{width:3px;background:#a7a7a9;height:35%}.signal i:nth-child(2){height:55%}.signal i:nth-child(3){height:75%}.signal i:nth-child(4){height:100%}
.brand{position:absolute;left:50%;top:3px;transform:translateX(-50%);width:55%;height:100%;display:grid;place-items:center;pointer-events:none}.brand-trace{position:absolute;inset:0;width:100%;height:100%;fill:none;stroke:#ff2020;stroke-width:2;filter:drop-shadow(0 0 4px #f00)}.brand-plate{position:relative;padding:10px 30px 14px;background:linear-gradient(#111113,#080809);clip-path:polygon(0 0,100% 0,94% 70%,86% 100%,14% 100%,6% 70%);border-bottom:1px solid #444;min-width:66%;text-align:center}.brand h1{white-space:nowrap;margin:0;font:italic 800 clamp(17px,2.4vw,37px)/1.4 Orbitron,sans-serif;letter-spacing:.025em;text-shadow:0 2px 1px #000,0 -1px 0 #fff5}.brand h1 span{color:var(--red);text-shadow:0 0 15px #f001}
.battery{display:flex;align-items:center;gap:10px;border:1px solid #3b3b3e;border-bottom-color:#202023;border-radius:12px;padding:9px 15px;background:linear-gradient(150deg,#111112,#040405);box-shadow:inset 0 1px 0 #ffffff0b;z-index:2}.battery>svg{width:25px;height:33px}.battery>div>span{font-size:clamp(13px,1.4vw,20px);font-weight:600;letter-spacing:.06em}.battery-segments{display:flex;gap:3px;margin-top:2px;transform:skewX(-18deg)}.battery-segments i{width:clamp(9px,1.1vw,18px);height:7px;background:#303034;border-top:1px solid #47474a}.battery-segments i:nth-child(-n+5){background:linear-gradient(#ff4141,#bd0707);border-top-color:#ff6767;box-shadow:0 0 7px #f001}
.deck{flex:1;min-height:0;display:flex;position:relative}.control-bay{width:44%;display:flex;align-items:center;justify-content:center;flex-direction:column;position:relative;z-index:1;--dial:min(40vw, calc(100dvh - 160px), 650px)}
.joystick{width:var(--dial);height:var(--dial);flex-shrink:0;position:relative;touch-action:none;border-radius:50%;outline:none;background:radial-gradient(circle,#101012 54%,#050505 68%,#242426 69%,#040405 70%,#131314 72%,transparent 72.5%);filter:drop-shadow(0 13px 15px #000)}.joystick:focus-visible{outline:1px solid #ff6868;outline-offset:5px}.joystick>div{pointer-events:none;position:absolute;border-radius:50%}.dial-ticks{inset:0;background:repeating-conic-gradient(from -1deg,#73737655 0deg .22deg,transparent .3deg 3deg);mask:radial-gradient(circle,transparent 67%,#000 67.3%,#000 70%,transparent 70.3%)}.outer-rim{inset:3%;border:1px solid #353538;box-shadow:0 0 0 3px #080809,inset 0 2px 2px #ffffff16,inset 0 -4px 5px #000;background:radial-gradient(circle,transparent 62%,#101012 69%,#050506 70%)}.light-arcs{inset:7%;background:conic-gradient(from 0deg,transparent 0 38deg,#ff3333 40deg 71deg,transparent 73deg 107deg,#f52525 109deg 140deg,transparent 142deg 218deg,#ed1818 220deg 251deg,transparent 253deg 287deg,#ff3333 289deg 320deg,transparent 322deg);mask:radial-gradient(circle,transparent 68%,#000 68.3% 69%,transparent 69.5%);filter:drop-shadow(0 0 5px red)}.inner-rim{inset:16%;background:linear-gradient(145deg,#aaa,#161618 16%,#080809 48%,#4a0808 77%,#9c2727);box-shadow:0 0 0 3px #030303,0 0 0 5px #3c1111,0 0 14px #e6000033,inset 0 1px 2px #ddd}.carbon-disc{inset:17.5%;border:3px solid #ff3030;background:radial-gradient(ellipse at 35% 5%,#ffffff10,transparent 46%),repeating-linear-gradient(45deg,#0000 0 3px,#0008 3px 6px),repeating-linear-gradient(-45deg,#242427 0 2px,#111113 2px 4px,#18181a 4px 6px);box-shadow:0 0 3px 1px #ff8989,0 0 0 3px #5e0909,inset 0 0 0 3px #18181b,inset 0 0 25px 12px #000,0 0 15px #f003}.direction-label{position:absolute;font-weight:700;font-size:calc(var(--dial)*.041);color:#858587;text-shadow:0 2px 2px #000;transition:color .15s,text-shadow .15s}.front{top:7.7%;left:50%;transform:translateX(-50%)}.rear{bottom:7%;left:50%;transform:translateX(-50%)}.left{left:10%;top:50%;transform:translate(-50%,-50%) rotate(-90deg)}.right{right:9.8%;top:50%;transform:translate(50%,-50%) rotate(90deg)}.arrow{position:absolute;width:9%;height:6%;fill:#bdbdc0;stroke:#f5f5f5;stroke-width:.5;filter:drop-shadow(0 3px 2px #000);transition:fill .15s,filter .15s;pointer-events:none}.arrow-front{top:23%;left:45.5%}.arrow-rear{bottom:23%;left:45.5%;transform:rotate(180deg)}.arrow-left{left:22%;top:47%;transform:rotate(-90deg)}.arrow-right{right:22%;top:47%;transform:rotate(90deg)}.knob-socket{inset:33%;background:#020203;box-shadow:0 0 0 2px #232325,0 6px 12px 7px #000,inset 0 2px 4px #9994}.knob{inset:35%;background:linear-gradient(135deg,#bababa,#181819 23%,#000 70%,#4d4d50);padding:3%;box-shadow:0 7px 10px 4px #000a;transition:transform .38s cubic-bezier(.2,1.5,.4,1);will-change:transform}.knob:before{content:'';position:absolute;inset:5%;border-radius:50%;border:2px solid #ff3838;box-shadow:0 0 7px #ff202099,inset 0 0 7px #ff202077}.knob>div{width:100%;height:100%;border-radius:50%;background:radial-gradient(ellipse at 35% 9%,#55555870,transparent 40%),radial-gradient(ellipse at 60% 110%,#a2151580,transparent 55%),repeating-linear-gradient(0deg,#ffffff02 0 1px,transparent 1px 3px),linear-gradient(145deg,#202023,#080809 70%);box-shadow:inset 0 2px 2px #ffffff45,inset 0 -5px 9px #000;border:1px solid #555558}.dragging .knob{transition:none}.active.direction-label{color:#f4eded;text-shadow:0 0 12px #ff3030}.active.arrow{fill:#fff;filter:drop-shadow(0 0 5px #ff3030)}.joystick[data-direction=frente] .carbon-disc{box-shadow:0 -7px 20px #f006,inset 0 0 25px 12px #000}.joystick[data-direction=re] .carbon-disc{box-shadow:0 7px 20px #f006,inset 0 0 25px 12px #000}.joystick[data-direction=esquerda] .carbon-disc{box-shadow:-7px 0 20px #f006,inset 0 0 25px 12px #000}.joystick[data-direction=direita] .carbon-disc{box-shadow:7px 0 20px #f006,inset 0 0 25px 12px #000}
.movement-plate{display:flex;gap:12px;align-items:center;justify-content:center;margin-top:3px;font-size:clamp(11px,1.1vw,16px);letter-spacing:.2em;font-weight:600;color:#a0a0a5}.movement-plate i{width:24px;height:1px;background:linear-gradient(90deg,transparent,#6d1818)}.movement-plate i:last-child{transform:rotate(180deg)}.movement-plate output{min-width:85px;text-align:center}.movement-plate.moving{color:#ff4747;text-shadow:0 0 12px #f005}
.reserved-panel{flex:1;position:relative;margin:20px 0 30px 4%;overflow:hidden;background:radial-gradient(ellipse at 55% 80%,#24242915,transparent 70%);border-top:1px solid #29292b40;border-bottom:1px solid #28282b50;clip-path:polygon(3% 0,100% 0,100% 95%,95% 100%,0 100%,0 4%)}.panel-seam{position:absolute;left:0;top:0;bottom:0;width:1px;background:linear-gradient(transparent,#4e4e5040,transparent)}.floor-grid{position:absolute;inset:40% -30% -50%;background:linear-gradient(90deg,#66666612 1px,transparent 1px),linear-gradient(#66666612 1px,transparent 1px);background-size:55px 55px;transform:perspective(300px) rotateX(57deg);mask-image:linear-gradient(transparent,#0006);opacity:.5}.registration{position:absolute;width:20px;height:20px;opacity:.4}.registration-top{top:12px;right:2px;border-top:1px solid #67676a;border-right:1px solid #67676a}.registration-bottom{bottom:12px;left:15px;border-bottom:1px solid #67676a;border-left:1px solid #67676a}.lower-trace{position:absolute;right:0;bottom:0;height:2px;width:45%;background:linear-gradient(90deg,transparent,#aa1010,red);box-shadow:0 0 12px #f006}.edge-details{height:22px;flex-shrink:0;display:flex;justify-content:space-between;align-items:center;font-size:9px;letter-spacing:.16em;color:#57575c}.demo-label{color:#8b6565}.rotate-screen{display:none}@keyframes connected{50%{box-shadow:inset 0 1px 2px #89ff92,0 0 17px #00ed2650}}
@media(max-height:500px) and (orientation:landscape){body{padding:8px 12px}.cockpit{padding:0 20px;border-radius:22px}.header{height:58px}.control-bay{--dial:min(40vw,calc(100dvh - 105px));width:43%}.brand-plate{padding:6px 15px 10px}.brand{width:51%}.brand h1{font-size:clamp(17px,2.4vw,24px)}.battery{padding:5px 9px;gap:7px;border-radius:9px}.battery>svg{width:20px;height:28px}.signal{height:19px;gap:2px}.signal i{width:2px}.connection{gap:7px}.connection-copy strong{font-size:13px}.connection-copy>span{font-size:12px}.wifi{margin-left:6px;width:25px}.movement-plate{font-size:10px;line-height:12px;margin-top:0}.edge-details{height:15px;font-size:7px}.reserved-panel{margin:14px 0 18px 4%}}
@media(orientation:portrait){.cockpit{display:none}.rotate-screen{height:100%;display:flex;align-items:center;justify-content:center;flex-direction:column;gap:22px;text-align:center;background:radial-gradient(ellipse,#31090955,transparent 65%)}.rotate-screen svg{width:75px;height:75px;color:#ff3434;filter:drop-shadow(0 0 12px #f004)}.rotate-screen h1{font:600 25px Rajdhani,sans-serif;margin:0}.rotate-screen span{font:italic 700 13px Orbitron,sans-serif;letter-spacing:.08em;color:#888}.rotate-screen b{color:var(--red)}}
@media(prefers-reduced-motion:reduce){*,*:before,*:after{animation:none!important;transition:none!important}}

/* Área livre, sem divisórias ou marcações laterais. */
.reserved-panel{border-top:0}
/* O halo acompanha a cor do estado da conexão. */
.connection-dot{--status-color:#06db2d;background:var(--status-color);animation:connection-pulse 1.8s ease-in-out infinite;box-shadow:0 0 8px var(--status-color)}
.connection.disconnected .connection-dot{--status-color:#ff3030}
.connection.disconnected .connection-copy strong{color:#ff6969}
@keyframes connection-pulse{0%,100%{opacity:1;box-shadow:0 0 5px var(--status-color),0 0 0 0 transparent;transform:scale(1)}50%{opacity:.7;box-shadow:0 0 15px var(--status-color),0 0 0 6px color-mix(in srgb,var(--status-color) 12%,transparent);transform:scale(.88)}}
.connection-dot{animation:connection-pulse 1.8s ease-in-out infinite!important}
/* Tela cheia e adaptação às áreas seguras do celular. */
body{height:100vh;height:100dvh;padding-top:max(clamp(8px,2vw,28px),env(safe-area-inset-top));padding-bottom:max(clamp(8px,2vw,28px),env(safe-area-inset-bottom))}
.fullscreen-button{position:relative;z-index:3;margin-left:auto;display:grid;place-items:center;width:44px;height:44px;flex-shrink:0;border:1px solid #38383b;border-radius:9px;color:#aaaab0;background:linear-gradient(145deg,#18181b,#070708);box-shadow:inset 0 1px 0 #ffffff12;cursor:pointer;touch-action:manipulation}
.fullscreen-button svg{width:22px;height:22px}.fullscreen-button:hover,.fullscreen-button:focus-visible{color:#fff;border-color:#ff3030;outline:none}.fullscreen-button:active{background:#301010}.collapse-icon{display:none}.fullscreen-button[aria-pressed=true] .expand-icon{display:none}.fullscreen-button[aria-pressed=true] .collapse-icon{display:block}
.fullscreen-hint{position:absolute;z-index:5;top:85px;right:20px;max-width:min(300px,calc(100% - 40px));padding:14px 18px;border:1px solid #762525;background:#141012f5;box-shadow:0 8px 30px #000;color:#e6dede;font-size:16px;border-radius:8px;pointer-events:none}
@media(orientation:portrait){
  body{padding:10px;padding-top:max(10px,env(safe-area-inset-top));padding-bottom:max(10px,env(safe-area-inset-bottom));padding-left:max(10px,env(safe-area-inset-left));padding-right:max(10px,env(safe-area-inset-right))}
  .cockpit{display:flex;padding:0 14px;border-radius:24px}
  .header{height:112px;align-items:flex-end;padding-bottom:12px}
  .brand{top:0;left:0;transform:none;width:100%;height:52px}
  .brand-plate{padding:6px 15px 10px;min-width:74%}.brand h1{font-size:clamp(15px,4.5vw,28px)}
  .connection{gap:9px;min-height:44px}.connection-copy strong{font-size:14px}.connection-copy>span{font-size:13px}.connection-dot{width:13px;height:13px}
  .deck{flex-direction:column}.reserved-panel{order:0;flex:1;min-height:0;margin:12px 0 10px;clip-path:none}.control-bay{order:1;width:100%;flex:0 0 auto;padding:0 0 12px;--dial:min(calc(100vw - 58px),calc(100dvh - 205px),520px)}
  .movement-plate{font-size:12px;margin-top:5px}.edge-details{height:20px;font-size:8px}.fullscreen-hint{top:125px}
}
@media(orientation:landscape) and (max-height:500px){body{padding-top:max(8px,env(safe-area-inset-top));padding-bottom:max(8px,env(safe-area-inset-bottom))}.fullscreen-button{width:44px;height:40px}.fullscreen-hint{top:65px}}
@media(orientation:landscape) and (max-height:330px){.header{height:48px}.control-bay{--dial:min(40vw,calc(100dvh - 95px))}.connection-copy strong{font-size:11px}.connection-copy>span{font-size:10px}}
.fullscreen-button[hidden]{display:none}
)CARRINHO";

const char APP_JS[] PROGMEM = R"CARRINHO('use strict';

// Protocolo HTTP compatível com o controle físico.
const DEMO_MODE = false;
const ESP32_BASE_URL = location.protocol === 'http:' && /^(192\.168\.|10\.|172\.(1[6-9]|2\d|3[01])\.)/.test(location.hostname) ? '' : 'http://192.168.10.103';
const joystick = document.querySelector('#joystick');
const knob = document.querySelector('#knob');
const movement = document.querySelector('#movement');
const connection = document.querySelector('.connection');
const connectionText = document.querySelector('.connection-copy strong');
document.querySelector('.connection-copy span').textContent = ESP32_BASE_URL ? new URL(ESP32_BASE_URL).hostname : location.hostname;

function setConnectionStatus(connected) {
  connection.classList.toggle('disconnected', !connected);
  connectionText.textContent = connected ? 'CONECTADO' : 'DESCONECTADO';
  connection.setAttribute('aria-label', `${connected ? 'Conectado' : 'Desconectado'}${DEMO_MODE ? ' — modo demonstração' : ' — ESP32'}`);
}

async function checkConnection() {
  if (!navigator.onLine) return setConnectionStatus(false);
  if (DEMO_MODE) return setConnectionStatus(true);
  try {
    const response = await fetch(`${ESP32_BASE_URL}/status`, {
      cache: 'no-store', signal: AbortSignal.timeout(2000),
    });
    setConnectionStatus(response.ok);
  } catch {
    setConnectionStatus(false);
  }
}
window.addEventListener('online', checkConnection);
window.addEventListener('offline', () => setConnectionStatus(false));
setConnectionStatus(false);
checkConnection();
if (!DEMO_MODE) setInterval(checkConnection, 5000);
const labels = { parar: 'PARADO', frente: 'FRENTE', re: 'RÉ', direita: 'DIREITA', esquerda: 'ESQUERDA' };
let currentCommand = 'parar';
let activePointer = null;
let commandInFlight = false;
let pendingCommand = null;
let pressedKeys = [];

async function sendCommand(command) {
  if (DEMO_MODE) {
    console.log('[DEMO]', command);
    return;
  }
  // Uma requisição por vez; só guarda o comando mais recente.
  pendingCommand = command;
  if (commandInFlight) return;
  commandInFlight = true;
  try {
    while (pendingCommand !== null) {
      const next = pendingCommand;
      pendingCommand = null;
      try {
        const response = await fetch(`${ESP32_BASE_URL}/comando?acao=${encodeURIComponent(next)}&origem=web`, {
          cache: 'no-store', signal: AbortSignal.timeout(800),
        });
        if (!response.ok) throw new Error(`HTTP ${response.status}`);
        setConnectionStatus(true);
      } catch (error) {
        setConnectionStatus(false);
        console.error('Falha ao enviar comando ao ESP32:', next, error);
        // Interrompe a interação e tenta enviar parar uma vez.
        if (next !== 'parar') stop();
      }
    }
  } finally {
    commandInFlight = false;
  }
}

// Renova o comando enquanto o joystick está pressionado.
// O firmware interrompe o movimento web após 1200 ms sem renovação.
if (!DEMO_MODE) setInterval(() => {
  if (currentCommand !== 'parar') sendCommand(currentCommand);
}, 250);

function setDirection(command) {
  if (command === currentCommand) return;
  currentCommand = command;
  joystick.dataset.direction = command;
  movement.value = labels[command];
  movement.parentElement.classList.toggle('moving', command !== 'parar');
  document.querySelectorAll('[data-highlight]').forEach(element => {
    element.classList.toggle('active', element.dataset.highlight === command);
  });
  sendCommand(command);
}

function moveKnob(dx, dy) {
  const radius = joystick.getBoundingClientRect().width * 0.18;
  const distance = Math.hypot(dx, dy);
  const scale = distance > radius ? radius / distance : 1;
  knob.style.transform = `translate(${dx * scale}px, ${dy * scale}px)`;
  const direction = distance < radius * 0.2 ? 'parar'
    : Math.abs(dx) > Math.abs(dy) ? (dx > 0 ? 'direita' : 'esquerda')
    : dy > 0 ? 're' : 'frente';
  setDirection(direction);
}

function movePointer(event) {
  const bounds = joystick.getBoundingClientRect();
  moveKnob(event.clientX - bounds.left - bounds.width / 2,
    event.clientY - bounds.top - bounds.height / 2);
}

function stop() {
  const pointer = activePointer;
  activePointer = null;
  pressedKeys = [];
  joystick.classList.remove('dragging');
  knob.style.transform = 'translate(0px, 0px)';
  setDirection('parar');
  if (pointer !== null && joystick.hasPointerCapture(pointer)) joystick.releasePointerCapture(pointer);
}

joystick.addEventListener('pointerdown', event => {
  if (activePointer !== null || (event.pointerType === 'mouse' && event.button !== 0)) return;
  event.preventDefault();
  pressedKeys = [];
  activePointer = event.pointerId;
  joystick.setPointerCapture(event.pointerId);
  joystick.focus({ preventScroll: true });
  joystick.classList.add('dragging');
  movePointer(event);
});
joystick.addEventListener('pointermove', event => {
  if (event.pointerId === activePointer) movePointer(event);
});
['pointerup', 'pointercancel', 'lostpointercapture'].forEach(type => {
  joystick.addEventListener(type, event => {
    if (event.pointerId === activePointer) stop();
  });
});
const keyVectors = { ArrowUp: [0, -1], ArrowDown: [0, 1], ArrowLeft: [-1, 0], ArrowRight: [1, 0] };
function updateKeyboard() {
  const vector = keyVectors[pressedKeys.at(-1)];
  if (!vector) return stop();
  const radius = joystick.getBoundingClientRect().width * 0.18;
  moveKnob(vector[0] * radius, vector[1] * radius);
}
joystick.addEventListener('keydown', event => {
  if (!keyVectors[event.key] || activePointer !== null) return;
  event.preventDefault();
  if (!pressedKeys.includes(event.key)) pressedKeys.push(event.key);
  updateKeyboard();
});
joystick.addEventListener('keyup', event => {
  if (!keyVectors[event.key] || activePointer !== null) return;
  event.preventDefault();
  pressedKeys = pressedKeys.filter(key => key !== event.key);
  updateKeyboard();
});
joystick.addEventListener('blur', stop);
window.addEventListener('blur', stop);
window.addEventListener('resize', stop);
window.addEventListener('pagehide', stop);
document.addEventListener('visibilitychange', () => { if (document.hidden) stop(); });

const fullscreenButton = document.querySelector('#fullscreen-button');
const fullscreenHint = document.querySelector('#fullscreen-hint');
let hintTimeout;
let automaticFullscreenAttempted = false;
const isStandalone = () => matchMedia('(display-mode: standalone)').matches || navigator.standalone === true;
const fullscreenElement = () => document.fullscreenElement || document.webkitFullscreenElement;

function showFullscreenHint(message) {
  clearTimeout(hintTimeout);
  fullscreenHint.textContent = message;
  fullscreenHint.hidden = false;
  hintTimeout = setTimeout(() => { fullscreenHint.hidden = true; }, 6500);
}

function updateFullscreenButton() {
  const expanded = Boolean(fullscreenElement()) || isStandalone();
  fullscreenButton.hidden = isStandalone();
  fullscreenButton.setAttribute('aria-pressed', String(expanded));
  const label = expanded ? 'Sair da tela cheia' : 'Entrar em tela cheia';
  fullscreenButton.setAttribute('aria-label', label);
  fullscreenButton.title = label;
  stop();
}

async function enterFullscreen(showHint = true) {
  if (fullscreenElement() || isStandalone()) return;
  const root = document.documentElement;
  try {
    if (root.requestFullscreen && document.fullscreenEnabled) {
      await root.requestFullscreen();
    } else if (root.webkitRequestFullscreen) {
      await root.webkitRequestFullscreen();
    } else {
      throw new Error('Fullscreen indisponível');
    }
  } catch {
    if (showHint) showFullscreenHint(/iPhone|iPad|iPod/.test(navigator.userAgent)
      ? 'Para abrir sem as barras: no Safari, toque em Compartilhar → Adicionar à Tela de Início e abra pelo ícone.'
      : 'Tela cheia indisponível neste navegador. Use a opção Adicionar à tela inicial ou abra em outro navegador.');
  }
}

fullscreenButton.addEventListener('click', async () => {
  automaticFullscreenAttempted = true;
  stop();
  if (fullscreenElement()) {
    try {
      if (document.exitFullscreen) await document.exitFullscreen();
      else if (document.webkitExitFullscreen) await document.webkitExitFullscreen();
    } catch { showFullscreenHint('Use o comando de voltar do navegador para sair da tela cheia.'); }
  } else await enterFullscreen();
});

// O navegador exige um gesto. Não altera a tela durante uma manobra.
document.addEventListener('click', event => {
  if (automaticFullscreenAttempted || !matchMedia('(pointer: coarse)').matches
    || event.target.closest('#joystick, #fullscreen-button')) return;
  automaticFullscreenAttempted = true;
  enterFullscreen(false);
});
document.addEventListener('fullscreenchange', updateFullscreenButton);
document.addEventListener('webkitfullscreenchange', updateFullscreenButton);
updateFullscreenButton();
)CARRINHO";

const char MANIFEST_JSON[] PROGMEM = R"CARRINHO({
  "name": "Carrinho IOT",
  "short_name": "Carrinho IOT",
  "lang": "pt-BR",
  "start_url": "./",
  "scope": "./",
  "display": "standalone",
  "background_color": "#050505",
  "theme_color": "#050505"
}
)CARRINHO";
