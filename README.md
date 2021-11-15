# ISA - TFTP client

Projekt pro předmět síťové aplikace

## Popis

Implementace klientské strany protokolu TFTP

## Omezení

Tato implementace klienta TFTP **nepodporuje** blocksizei, multicast a timeout.
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


