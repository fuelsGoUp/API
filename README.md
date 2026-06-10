# API de Gestão de tarefas

Para usar esta API é necessario ter em sua maquina:

* PostgreSQL
* Compilador C++
* Cmake
* vcpkg
* instalar crow e libpqxx no powershell pelo vcpkg
* extensões c++ no visual studio code
* git para clonar o projeto

Para instalar as dependencias pelo vcpkg, basta usar o comando `vcpkg install crow lobpqxx` no poweshell.

Clone a API com git e entrar na pasta da API:

`git clone https://github.com/fuelsGoUp/API`

`cd API`

Então podemos compilar o projeto usando cmake:

`cmake --build build`

Este comando vai criar uma pasta build, para separar o .exe do projeto, deixando tudo mais organizado para alterações caso seja necessário:

`cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake` Ou o caminho do vcpkg até o vcpkg.cmake que você tenha feito a instalação.

Então poderemos mandar o cmake compilar:

`cmake --build build`

E podemos rodar pela linha de comando no diretorio API:

`.\build\Debug\TaskManager.exe` ou `.\build\TaskManager.exe` caso não tenha a pasta debug.








