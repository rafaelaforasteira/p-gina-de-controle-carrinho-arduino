from pathlib import Path

root = Path(__file__).resolve().parents[1]
assets = {
    "index.html": "INDEX_HTML",
    "styles.css": "STYLES_CSS",
    "app.js": "APP_JS",
    "manifest.webmanifest": "MANIFEST_JSON",
}
header = "#pragma once\n#include <Arduino.h>\n\n// Gerado a partir dos arquivos da raiz. Execute python3 scripts/embed_web.py.\n"
for filename, name in assets.items():
    content = (root / filename).read_text(encoding="utf-8")
    if ')CARRINHO"' in content:
        raise ValueError("Delimitador reservado em " + filename)
    header += f'\nconst char {name}[] PROGMEM = R"CARRINHO({content})CARRINHO";\n'
(root / "firmware/carrinho/web_assets.h").write_text(header, encoding="utf-8")
print("web_assets.h atualizado")
