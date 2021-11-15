# ISA - TFTP client

Projekt pro předmět síťové aplikace

## Popis

Implementace klientské strany protokolu TFTP

## Omezení

Tato implementace klienta TFTP **nepodporuje** blocksize, multicast a timeout.
Tedy zadání možností `-t`, `-m` a `-s` vrátí error.

## Příklad spuštění
Pokud byl proveden příkaz `make`, v kořenové složce se bude nacházet soubor `mytftpclient`.
Tento soubor se spouští jako `$ ./mytftpclient`. Po spuštení lze psát do vlastní příkazové
řádky aplikace.

Základní přiklad spuštění vypadá například takto:

```
$ ./mytftpclient
> -R -d file.txt
> ...
```

Spuštění s více možnostmi by mohlo vypadat například takto:

```
$ ./mytftpclient
> -W -d file.txt -a ::1,69 -c octet
> ...
```

## Seznam odevzdaných souborů

```
README.md
Makefile
src/main.cpp
src/input_structure.cpp
src/input_structure.hpp
src/parse_input.cpp
src/parse_input.hpp
src/tftp_client.cpp
src/tftp_client.hpp
```
