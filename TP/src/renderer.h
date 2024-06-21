#include "arvore.h"
#include <gtk/gtk.h>

/* Botao que fecha o programa.
 * Esse botão faz parte da segunda janela.
 */
void on_btnExit_clicked(GtkButton *btnExit, gpointer *data);
void on_btnSearch_clicked(GtkButton *btnSearch, gpointer *data);

/* Botão da segunda janela que abre uma janela
 * que pesquisa um registro na árvore.
 */
void on_btnPesquisa_clicked(GtkButton *btnPesquisar, gpointer *data);

/* Botão da janelaRemover
 * ao ser clicado ele remove um registro da árvore.
 */
void on_btnRetirar_clicked(GtkButton *btnRetira, gpointer *data);

/* Botão da janela 2 que abre uma janela para
 * para remover um registro.
 */
void on_btnRemover_clicked(GtkButton *btnRemover, gpointer *data);

/* Botão que insere um registro na árvore.
 */
void on_btnSubmeter_clicked(GtkButton *btnSubmeter, gpointer *data);

/* Botao que abre uma janela para inserir um registro 
 * Esse botão faz parte da segunda janela.
 */
void on_btnInsere_clicked(GtkButton *btnInsere, gpointer *data);

/* Botao para pegar o nome do arquivo com os registros.
 * Esse botao faz parte da primeira janela.
 * Aqui abrimos o arquivo de registros e inserimos na arvore B
 */
void on_btnReg_clicked(GtkButton *btnReg, gpointer *data);


void render();

