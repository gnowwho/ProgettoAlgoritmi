//NON FUNZIONANTE

void allocavettore (int n; int **pV)
{
  *pV = (int*) malloc (n*sizeof(int));
  ......
}

//nel main avrò

int int main(int argc, char const *argv[]) {
  int *V;
  int n;

  n=caricadati();  //questa funzione non esiste
 allocavettore(n,&V);

  return 0;
}

/*Perchè usare questa sintassi?
il vantaggio è che così possiamo allocare più vettori contemporaneamente:
è utile in situazioni in cui carichiamo da file, per esempio*/
