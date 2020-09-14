#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>
#include<sys/types.h>
#include<sys/dir.h>
#include<dirent.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

int main(int argc, char **argv)
{
    int secenek;
    while(secenek != 9){
      printf("%s\n", "---------SEÇENEKLER---------");
      printf("%s\n", "1) CONFIG DOSYASI OLUSTUR");
      printf("%s\n", "2) CONFIG DOSYASI SİL");
      printf("%s\n", "3) CONFIG DOSYASI BUL(ARA)");
      printf("%s\n", "4) CONFIG DOSYASINI DEĞİŞTİR");
      printf("%s\n", "9) ÇIKIŞ");
      printf("%s", "Seçeneğini gir: ");
      scanf("%d", &secenek);

      if( secenek == 1 ){
        char fileName[100];
        printf("%s", "Dosya ismini gir(uzantısı ile): ");
        scanf("%s", fileName);

        FILE * fPtr;
        fPtr = fopen(fileName, "w+");
        if(fPtr == NULL)
        {
            /* File not created hence exit */
            printf("ERROR!!!!!!!!! DOSYA OLUŞTURULAMIYOR!\n");
        }
        else{
          printf("%s\n", "CONFIG DOSYASI OLUŞTURULDU.");
        }
        fclose(fPtr);

        printf("%s\n", "ANA MENÜYE DÖNÜLÜYOR...");
      }
      else if( secenek == 2 ){
        char fileName[100];
        printf("%s", "Silinecek config dosyası ismini gir(uzantısı ile): ");
        scanf("%s", fileName);

        int del = remove(fileName);
        if (!del){
          printf("DOSYA SİLİNDİ...");

        }
        else{
          printf("DOSYA SİLİNEMİYOR...");      
        }
        printf("%s\n", "ANA MENÜYE DÖNÜLÜYOR...");
      }
      else if(secenek == 3){
        char fileName[100];
        printf("%s", "Dosya ismini gir(uzantısı ile): ");
        scanf("%s", fileName);

        if( access( fileName, F_OK ) != -1 ) {
          printf("%s\n", "BU DOSYA MEVCUT, BULUNDU");
            // file exists
        } else {
            // file doesn't exist
          printf("%s\n", "CONFIG DOSYASI BULUNAMADI!");
        }
      }
      else if( secenek == 4){
        char fileName[100];
        strcpy(fileName, "test.cfg");
        if( access( fileName, F_OK ) != -1 ) {
          while(secenek != 9){
            printf("%s\n", "BU DOSYAYA NE YAPMAK İSTERSİN?");
            printf("%s\n", "1) DEĞER EKLE");
            printf("%s\n", "2) DEĞER SİL");
            printf("%s\n", "3) DEĞER ARA");
            printf("%s\n", "4) DEĞER DEĞİŞTİR");
            printf("%s\n", "9) ÇIKIŞ");
            printf("%s", "Seçeneğini gir: ");
            scanf("%d", &secenek);

            config_t cfg;
            config_setting_t *root, *setting, *group, *array;
            config_init(&cfg);
            config_set_options(&cfg,
                               (CONFIG_OPTION_SEMICOLON_SEPARATORS
                                | CONFIG_OPTION_COLON_ASSIGNMENT_FOR_GROUPS
                                | CONFIG_OPTION_OPEN_BRACE_ON_SEPARATE_LINE));

            /* Read the file. If there is an error, report it and exit. */
            if(! config_read_file(&cfg, fileName))
            {
              fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
                      config_error_line(&cfg), config_error_text(&cfg));
              config_destroy(&cfg);
              return(EXIT_FAILURE);
            }

            if( secenek == 1 ){
              root = config_root_setting(&cfg);
              while(1){
                char gs;
                printf("Grup mu String mi? (g-s)");
                scanf("%c", &gs);

                if(gs == 'g' || gs == 'G'){
                  char key[100];
                  printf("%s ", "KEY: ");
                  scanf("%s", key);  
                 
                  if( config_setting_get_member (root, key) == NULL ){
                    root = config_setting_add(root, key, CONFIG_TYPE_GROUP); 
                  } 
                  else{
                    root = config_setting_get_member (root, key);
                  }
            
                }
                else if( gs == 's' || gs == 'S' ){
                  char key[100];
                  printf("%s ", "KEY: ");
                  scanf("%s", key);   

                  char value[100];
                  printf("%s", "VALUE: ");
                  scanf("%s", value);

                  root = config_setting_add(root, key, CONFIG_TYPE_STRING);
                  config_setting_set_string(root, value);
                  break;
                }
              }

            }
            else if( secenek == 2 ){
              root = config_root_setting(&cfg);

              while(1){
                printf("Direct Sil | Indirect Sil (d, i): ");
                char di;
                scanf(" %c", &di);

                if( di == 'd' || di == 'D' ){
                  char key[100];
                  printf("%s ", "KEY: ");
                  scanf("%s", key);


                  config_setting_remove (root, key);

                  break;
                }
                else{
                  char key[100];
                  printf("%s ", "KEY: ");
                  scanf("%s", key);

                  root = config_setting_get_member (root, key);
                }
              }
            }
            else if( secenek == 3 ){
                char key[100];
                char keyMain[100];
                int flag = 0;
                while(1){
                  printf("%s ", "KEY: ");
                  scanf("%s", key);
                  strcat(keyMain, key);
                  if( flag == 0){
                    setting = config_lookup(&cfg, key);
                  }
                  else{
                    setting = config_lookup(&cfg, keyMain);
                  }
                  
                  if(setting != NULL){
                    printf("%s\n", "BU ANAHTAR MEVCUT");
                  }
                  else{
                    printf("%s\n", "BU ANAHTAR MEVCUT DEĞİL!!");
                  }     
                  printf("%s", "Child verileri incelemek istiyor musun? (e/h) ");

                  char evha;
                  scanf(" %c", &evha);
                  if(evha == 'h' || evha == 'H'){
                    break;
                  }
                  else{
                    if( flag == 0 ){
                      strcpy(keyMain, key);
                    }
                    
                    strcat(keyMain, ".");  
                  }
                  flag++;
                }

            }
            else if( secenek == 4 ){

                char key[100];
                char keyMain[100];
                int flag = 0;
                while(1){
                  printf("%s ", "KEY: ");
                  scanf("%s", key);
                  strcat(keyMain, key);
                  if( flag == 0){
                    setting = config_lookup(&cfg, key);
                  }
                  else{
                    setting = config_lookup(&cfg, keyMain);
                  }
                  
                  if(setting != NULL){
                    printf("%s\n", "BU ANAHTAR MEVCUT");
                  }
                  else{
                    printf("%s\n", "BU ANAHTAR MEVCUT DEĞİL!!");
                  }     
                  printf("%s", "Child verileri incelemek istiyor musun? (e/h) ");

                  char evha;
                  scanf(" %c", &evha);
                  if(evha == 'h' || evha == 'H'){
                    char value[100];
                    printf("%s", "VALUE(bu değer aranan değer ile değiştirilecek): ");
                    scanf("%s", value);

                    config_setting_set_string(setting, value);

                    break;
                  }
                  else{
                    if( flag == 0 ){
                      strcpy(keyMain, key);
                    }
                    
                    strcat(keyMain, ".");  
                  }
                  flag++;
                }

            
            }

            /* Write out the new configuration. */
            if(! config_write_file(&cfg, fileName))
            {
              fprintf(stderr, "Error while writing file.\n");
              config_destroy(&cfg);
              return(EXIT_FAILURE);
            }
            config_destroy(&cfg);
          }
            // file exists
          } 
          else {
            // file doesn't exist
            printf("%s\n", "CONFIG DOSYASI BULUNAMADI!");
          }
      }
      sleep(1);
  }
  printf("%s\n", "Exit...");
  return(0);
}
/* eof */