#include "Model3D.h"

// такая фукция уже есть в файле Program.c
static inline char* getFileText(const char* path)
{
   FILE* file = fopen(path, "rb");
   if (file == 0)
   {
       printf("%s \"%s\" %s\n", "File", path, "not found.");
   }

   fseek(file, 0, SEEK_END);
   unsigned file_lentgh = ftell(file);
   char* text = malloc(file_lentgh + 1);
   memset(text, 0, file_lentgh + 1);

   fseek(file, 0, SEEK_SET);
   fread(text, 1, file_lentgh, file);
   fclose(file);

   return text;
}

void model3DInit(struct Model3D* model, const char* file_name)
{
    char* text = getFileText(file_name);



    free(text);
}
