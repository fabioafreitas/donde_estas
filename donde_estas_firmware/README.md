# Passo a Passo para instalação de fábrica

1. Plugue o ESP32 no USB
2. Abra o projeto no platformIo
3. Preencha no arquivo /data/config.json o nome do device virtual do thingsboard no campo "smartrural_device_id" e o seu access token no campo "thingsboard_http_access_token"
4. Na opção "Platform", use o comando "Erase Flash", para resetar o ESP32
5. Na opção "Platform", use o comando "Upload Filesystem Image", para transferir os arquivos da pasta /data para o ESP32
6. Na opção "General", use o comando "Upload", para instalar o firmware no ESP32
7. Desplugue o ESP32 do USB, monte-o no respectivo aviquality e teste seu funcionamento