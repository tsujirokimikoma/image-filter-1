/**************************************************************************
*                                                                         *
*           Universidade Federal do Maranhão                              *
*       Departamento de Engenharia da Computação                          *
*                                                                         *
*  Author: Prof. Dr. Thales Levi Azevedo Valente                           *
*                                                                         *
*  Description: Processamento de Imagens em C                              *
*                                                                         *
*  Date: 27-11-2023                                                       *
*                                                                         *
* Este material fornece um programa simples para processamento de imagens *
* em C, incluindo filtros como negativo, escala de cinza, blur e Sobel.    *
*                                                                         *
* Conteúdos do Material:                                                  *
*   1. Carregamento de uma imagem em C                                    *
*   2. Aplicação de filtros: negativo, escala de cinza, blur e Sobel       *
*   3. Salvamento da imagem processada                                    *
*   4. Manipulação de Dados e Ponteiros                                   *
*   5. Criação de Menu                                                   *
*                                                                         *
***************************************************************************
* -------------------------------------------------------------------------*
*   IMPORTANTE:                                                           *
*   1- NÃO modifique a assinatura das funções (tipo retorno, nome, parâm.)*
*   2- Apenas implemente o corpo das funções trocando o ; por chaves      *
*   3- NÃO modifique a main                                               *
*   4- Veja o arquivo "exemplo.c"                                         *
*   5- NOME DO ARQUIVO É "NOME_SOBRENOME1_SOBRENOME2"                     *
* -------------------------------------------------------------------------*
*                                                                         *
* Inicie o código abaixo para começar a exploração.                       *
*                                                                         *
***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void applyNegativeFilter(unsigned char *img, int width, int height, int channels);
void applyGrayScaleFilter(unsigned char *img, int width, int height, int channels);
void applyBlurFilter(unsigned char *img, int width, int height, int channels);
void applySobelFilter(unsigned char *img, int width, int height, int channels);
void saveImage(const unsigned char *img, int width, int height, int channels, const char *outputFileName);

int main() {
    int width, height, channels;
    char inputFile[100] = "filtro.jpg";
    char outputFileName[100];

while (1){
    // Carregar a imagem
    unsigned char *img = stbi_load(inputFile, &width, &height, &channels, 0);
    if (img == NULL) {
        fprintf(stderr, "Erro ao carregar a imagem.\n");
        return 1;
    }

    int choice;
    printf("Escolha o filtro a ser aplicado:\n");
    printf("1: Negativo\n");
    printf("2: Escala de Cinza\n");
    printf("3: Blur\n");
    printf("4: Sobel\n");
    printf("Digite sua escolha: ");
    scanf("%d", &choice);

    switch(choice) {
        case 1:
            applyNegativeFilter(img, width, height, channels);
            strcpy(outputFileName, "negativo.jpg");
            break;
        case 2:
            applyGrayScaleFilter(img, width, height, channels);
            strcpy(outputFileName, "escala_de_cinza.jpg");
            break;
        case 3:
            applyBlurFilter(img, width, height, channels);
            strcpy(outputFileName, "blur.jpg");
            break;
        case 4:
            applyGrayScaleFilter(img, width, height, channels); // Para Sobel, primeiro converte para escala de cinza
            applySobelFilter(img, width, height, channels);
            strcpy(outputFileName, "sobel.jpg");
            break;
        default:
            printf("Escolha inválida!\n");
            stbi_image_free(img);
            return 1;
    }

    // Salvar a imagem processada com o nome apropriado
    saveImage(img, width, height, channels, outputFileName);

    // Liberar memória
    stbi_image_free(img);
}
    return 0;
}

void applySobelFilter(unsigned char *img, int width, int height, int channels) {
  int soma1;
  int soma2;
  float kernelx[3][3]={{ -1,  0,  1 },//inicializando o kernel responsável por Sobelizar na horizontal
                    { -2,  0,  2 },
                    { -1,  0,  1 }};
  float kernely[3][3]={{ -1,  -2,  -1 },//inicializando o kernel responsável por Sobelizar na vertical
                    { 0,  0,  0 },
                    { 1,  2,  1 }};
      //Mesma coisa do Blur, por enquanto
       unsigned char *temp = malloc(width * height * 3); //iniciando a imagem temporária, para ser aplicada no processo de soma.
       unsigned char *temp2 = malloc(width * height * 3); //iniciando a imagem temporária, para ser aplicada no processo de sobel

      for (int i = 0; i < width * height * 3; ++i) {
          temp[i] = img[i]; // torna temporário = Imagem original
      }
      for (int i = 0; i < width * height * 3; ++i) {
          temp2[i] = img[i]; // torna temporário 2 = Imagem original
      }

       for (int i = 1; i < height - 1; ++i) {
          for (int j = 1; j < width - 1; ++j) {
          for (int k = 0; k < 3; ++k) {
               soma1 = 0;
                  soma2 = 0;
              for (int di = -1; di <= 1; ++di) {
                  for (int dj = -1; dj <= 1; ++dj) {// processo de obtenção da soma e  aplicação do Kernel horizontal na soma, aplicando a parte horizontal do Sobel
                          soma1 +=  kernelx[dj+1][di+1] * temp[((i + di)* width + (j + dj)) * 3 + k];
                          soma2 +=  kernely[dj+1][di+1] * temp[((i + di)* width + (j + dj)) * 3 + k];
          }
          }
         // Calcular a magnitude da borda
                  int magnitude = (int)sqrt((soma1 * soma1) + (soma2 * soma2));

  // limitar a magnitude a 255 ( valor máximo do byte)
  if(magnitude>255){
      magnitude = 255;
  }
                  // Atribuir o valor na imagem temporaria
                  temp2[(i * width + j) * 3 + k] = (unsigned char)magnitude;
  img[(i * width + j ) * 3 + k] = temp2[(i * width + j) * 3 + k];
          }
          }
       }
}
void saveImage(const unsigned char *img, int width, int height, int channels, const char *outputFileName) {
    char outputPath[200];
    sprintf(outputPath, "Y:\\\\eng\\\\%s", outputFileName); // Constrói o caminho no diretório atual
    stbi_write_jpg(outputPath, width, height, channels, img, 100);
    printf("%s", outputPath);
}


void applyNegativeFilter(unsigned char *img, int width, int height, int channels) {
  //Laço para percorrer a imagem byte por byte
  for (int i=0; i<height*width*channels; i++){
    img[i]= 255-img[i];
  }
    // Implementação do Filtro Negativo
}

void applyGrayScaleFilter(unsigned char *img, int width, int height, int channels) {
    // Implementação do filtro de escala de cinza
  // Gray = 0.3*R + 0.59*G + 0.11*B

  //Tranformar tudo em cinza
  for (int k=0; k<width*height*channels; k+= channels){
    unsigned char Cinza = (unsigned char) (0.3*img[k] + 0.59*img[k+1] + 0.11*img[k+2]);/// Fórmula de conversão RGB para Cinza
    ///Aplicar a trasnferência cinza nas cores RGB da imagem
    img[k]= Cinza; //Aplicando para o canal RED
    img[k+1]= Cinza; //Aplicando para o canal GREEN
    img[k+2]= Cinza; //Aplicando para o canal BLUE
  }
  
}

void applyBlurFilter(unsigned char *img, int width, int height, int channels) {
    // Implementação do filtro de blur

  float v=1.0 / 9.0;
  float kernel[3] [3] = {{v, v, v},
              {v, v, v},
              {v, v, v}};
    unsigned char *temp = malloc (width * height * 3); //iniciando a imagem temporária, para ser aplicada no processo de soma.
for (int i = 0; i< width *height * 3; ++i){
  temp[i] = img[i]; //torna temporário = imagem original
}
  
for (int i= 1; i< height - 1; ++i) {
    for (int j= 1; j< width - 1; ++j) {
      for (int k= 0; k< channels; ++k) {
        int soma = 0;
        for (int di =-1; di <= 1; ++di) {
          for (int dj =-1; dj <= 1; ++dj) {//Processo de obtenção da soma e aplicação do Kernel na soma, borrando a imagem temporária
            soma+= kernel [dj+1][di+1] * temp[((i+di) * width + (j+dj)) * 3 +k];
        }
  }// criando a imagem toda borrada.
  img [(i * width + j) * 3 + k] = soma;
      }
    }
}
}
