#include "renderer.h"

/* Variaveis globais usadas na aplicação */
GtkWidget *txtReg;

GtkWidget *txtNome;
GtkWidget *txtCpf;
GtkWidget *lblSucesso;
GtkWidget *lblCompins;

GtkWidget *lblAltura;
GtkWidget *lblAcessos;
GtkWidget *lblInserir;
GtkWidget *lblPesquisar;
GtkWidget *lblRemover;

GtkWidget *txtRemove;
GtkWidget *lblRemovido;
GtkWidget *lblComprem;

GtkWidget *txtPesquisar;
GtkWidget *lblAchado;
GtkWidget *lblCompPes;

TipoPagina *Dicionario;

int numReg;

/* Botao que fecha o programa.
 * Esse botão faz parte da segunda janela.
 */
void on_btnExit_clicked(GtkButton *btnExit, gpointer *data)
{
	gtk_main_quit();
}

void on_btnSearch_clicked(GtkButton *btnSearch, gpointer *data)
{
	int compPes = 0;
	char *strInt;
	char str[15] = {0};
	TipoRegistro x;

	/* Pega o cpf digitado e armazena numa string*/
	const gchar *strPes = gtk_entry_get_text(GTK_ENTRY(txtPesquisar));

	strInt = (char*) malloc(sizeof(strPes)+1);
	strcpy(strInt, strPes);

	/* Faz a conversao da string para inteiro e retira a chave */
	strcpy(x.Chave.nome, "xxxxxxxx");
	strcpy(x.Chave.cpf, strInt);

	if(Pesquisa(&x, Dicionario, &compPes) == 1)
		gtk_label_set_text(GTK_LABEL(lblAchado), "Registro encontrado!");
	else
		gtk_label_set_text(GTK_LABEL(lblAchado), "Registro não encontrado!");
	
	sprintf(str, "%d", compPes);
	gtk_label_set_text(GTK_LABEL(lblCompPes), str);
	
	/* Parte do código dedicada em atualizar
	 * o label da janela anterior com o total
	 * de comparações.
	 */
	strcpy(str, gtk_label_get_text(GTK_LABEL(lblPesquisar)));
	compPes += StringToInt(str);
	sprintf(str, "%d", compPes);
	gtk_label_set_text(GTK_LABEL(lblPesquisar), str);
}
/* Botão da segunda janela que abre uma janela
 * que pesquisa um registro na árvore.
 */
void on_btnPesquisa_clicked(GtkButton *btnPesquisar, gpointer *data)
{
	GtkWidget *janelaPesquisa;
	GtkBuilder *builderPes;

	builderPes = gtk_builder_new();
	gtk_builder_add_from_file(builderPes, "glade/janelaPesquisa.glade", NULL);

	janelaPesquisa = GTK_WIDGET(gtk_builder_get_object(builderPes, "janelaPesquisar"));
	txtPesquisar = GTK_WIDGET(gtk_builder_get_object(builderPes, "txtPesquisar"));
	lblAchado = GTK_WIDGET(gtk_builder_get_object(builderPes, "lblAchado"));
	lblCompPes = GTK_WIDGET(gtk_builder_get_object(builderPes, "lblCompPes"));

	gtk_builder_connect_signals(builderPes, NULL);
	g_object_unref(builderPes);

	gtk_widget_show(janelaPesquisa);
}

/* Botão da janelaRemover
 * ao ser clicado ele remove um registro da árvore.
 */
void on_btnRetirar_clicked(GtkButton *btnRetira, gpointer *data)
{
	int compRem = 0;
	int altura;
	char *strInt;
	char str[15] = {0};
	TipoRegistro x;

	/* Pega o cpf digitado e armazena numa string*/
	const gchar *strRem = gtk_entry_get_text(GTK_ENTRY(txtRemove));

	strInt = (char*) malloc(sizeof(strRem)+1);
	strcpy(strInt, strRem);

	/* Faz a conversao da string para inteiro e retira a chave */
	strcpy(x.Chave.nome, "xxxxxxxx");
	strcpy(x.Chave.cpf, strInt);

	Retira(x.Chave, &Dicionario, &compRem);
	numReg--;

	gtk_label_set_text(GTK_LABEL(lblRemovido), "Registro removido com sucesso!");
	
	sprintf(str, "%d", compRem);
	gtk_label_set_text(GTK_LABEL(lblComprem), str);
	
	/* Parte do código dedicada em atualizar
	 * o label da janela anterior com o total
	 * de comparações.
	 */
	strcpy(str, gtk_label_get_text(GTK_LABEL(lblRemover)));
	compRem += StringToInt(str);
	sprintf(str, "%d", compRem);
	gtk_label_set_text(GTK_LABEL(lblRemover), str);
	altura = Altura((numReg+1)/2);
	sprintf(str, "%d", altura);
	gtk_label_set_text(GTK_LABEL(lblAltura), str);
	gtk_label_set_text(GTK_LABEL(lblAcessos), str);
}

/* Botão da janela 2 que abre uma janela para
 * para remover um registro.
 */
void on_btnRemover_clicked(GtkButton *btnRemover, gpointer *data)
{
	GtkWidget *janelaRemover;
	GtkBuilder *builderRem;

	builderRem = gtk_builder_new();
	gtk_builder_add_from_file(builderRem, "glade/janelaRemover.glade", NULL);

	janelaRemover = GTK_WIDGET(gtk_builder_get_object(builderRem, "janelaRemover"));
	txtRemove = GTK_WIDGET(gtk_builder_get_object(builderRem, "txtRemove"));
	lblRemovido = GTK_WIDGET(gtk_builder_get_object(builderRem, "lblRemovido"));
	lblComprem = GTK_WIDGET(gtk_builder_get_object(builderRem, "lblCompRem"));

	gtk_builder_connect_signals(builderRem, NULL);
	g_object_unref(builderRem);

	gtk_widget_show(janelaRemover);
}

/* Botão que insere um registro na árvore.
 */
void on_btnSubmeter_clicked(GtkButton *btnSubmeter, gpointer *data)
{
	int compIns = 0;
	int altura;
	TipoRegistro x;
	char str[30] = {0};
	char *strInt;
	const gchar *strNome = gtk_entry_get_text(GTK_ENTRY(txtNome));
	const gchar *strCpf = gtk_entry_get_text(GTK_ENTRY(txtCpf));
	//const gchar *strData = gtk_entry_get_text(GTK_ENTRY(txtData));

	strInt = (char*) malloc(sizeof(strCpf)+1);
	strcpy(strInt, strCpf);

	strcpy(x.Chave.nome, strNome);
	strcpy(x.Chave.cpf, strInt);
	Insere(x, &Dicionario, &compIns);
	numReg++;

	gtk_label_set_text(GTK_LABEL(lblSucesso), "Registro inserido com sucesso!");
	
	sprintf(str, "%d", compIns);
	gtk_label_set_text(GTK_LABEL(lblCompins), str);
	
	/* Parte do código dedicada em atualizar
	 * o label da janela anterior com o total
	 * de inserções.
	 */
	strcpy(str, gtk_label_get_text(GTK_LABEL(lblInserir)));
	compIns += StringToInt(str);
	sprintf(str, "%d", compIns);
	gtk_label_set_text(GTK_LABEL(lblInserir), str);
	altura = Altura((numReg+1)/2);
	sprintf(str, "%d", altura);
	gtk_label_set_text(GTK_LABEL(lblAltura), str);
	gtk_label_set_text(GTK_LABEL(lblAcessos), str);
}

/* Botao que abre uma janela para inserir um registro 
 * Esse botão faz parte da segunda janela.
 */
void on_btnInsere_clicked(GtkButton *btnInsere, gpointer *data)
{
	GtkWidget *janelaInsere;
	GtkBuilder *builder3;

	builder3 = gtk_builder_new();
	gtk_builder_add_from_file(builder3, "glade/janelaInsere.glade", NULL);

	janelaInsere = GTK_WIDGET(gtk_builder_get_object(builder3, "janelaInsere"));

	txtNome = GTK_WIDGET(gtk_builder_get_object(builder3, "txtNome"));
	txtCpf = GTK_WIDGET(gtk_builder_get_object(builder3, "txtCpf"));
	lblSucesso = GTK_WIDGET(gtk_builder_get_object(builder3, "lblSucesso"));
	lblCompins = GTK_WIDGET(gtk_builder_get_object(builder3, "lblCompins"));
	//txtData = GTK_WIDGET(gtk_builder_get_object(builder3, "txtData"));

	gtk_builder_connect_signals(builder3, NULL);
	g_object_unref(builder3);

	gtk_widget_show(janelaInsere);
}

/* Botao para pegar o nome do arquivo com os registros.
 * Esse botao faz parte da primeira janela.
 * Aqui abrimos o arquivo de registros e inserimos na arvore B
 */
void on_btnReg_clicked(GtkButton *btnReg, gpointer *data)
{
	GtkWidget *janela2;
	GtkBuilder *builder2;

	FILE *aqrReg;
	char *nomeArq;
	char strCount[10] = {0};
	TipoRegistro x;
	int altura;
	int compIns = 0;

	numReg = 0;

	const gchar *strReg = gtk_entry_get_text(GTK_ENTRY(txtReg));

	nomeArq = (char*) malloc(sizeof(strReg)+1);

	strcpy(nomeArq, strReg);
	
	aqrReg = fopen(nomeArq, "r+");
	
	/*Verifica se houve erro ao abrir o arquivo*/
	if(aqrReg == NULL)
	{
		const gchar *label = "Falha!";
		gtk_button_set_label(btnReg, label);
		gtk_main_quit();
	}
	else{
		/* Cria o dicionario */
		while(fscanf(aqrReg, "%s %s", x.Chave.nome, x.Chave.cpf) != EOF) 
		{ 
			Insere(x, &Dicionario, &compIns);
			numReg++;
		}

		builder2 = gtk_builder_new();
		gtk_builder_add_from_file(builder2, "glade/janela2.glade", NULL);

		janela2 = GTK_WIDGET(gtk_builder_get_object(builder2, "janela2"));

		lblAltura = GTK_WIDGET(gtk_builder_get_object(builder2, "lblAltura"));
		lblAcessos = GTK_WIDGET(gtk_builder_get_object(builder2, "lblAcessos"));
		lblInserir = GTK_WIDGET(gtk_builder_get_object(builder2, "lblInserir"));
		lblRemover = GTK_WIDGET(gtk_builder_get_object(builder2, "lblRemover"));
		lblPesquisar = GTK_WIDGET(gtk_builder_get_object(builder2, "lblPesquisar"));


		altura = Altura((numReg+1)/2);
		sprintf(strCount, "%d", altura);
		gtk_label_set_text(GTK_LABEL(lblAltura), strCount);
		gtk_label_set_text(GTK_LABEL(lblAcessos), strCount);
		sprintf(strCount, "%d", compIns);
		gtk_label_set_text(GTK_LABEL(lblInserir), strCount);


		gtk_builder_connect_signals(builder2, NULL);
		g_object_unref(builder2);

		gtk_widget_show(janela2);
		//gtk_main();
	}
}

void render(int argc, char *argv[])
{
	/*Cria a primeira janela a ser mostrada */
	GtkWidget *janela1;
	GtkBuilder *builder;

	gtk_init(&argc, &argv);

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "glade/janela1.glade", NULL);

	janela1 = GTK_WIDGET(gtk_builder_get_object(builder, "janela1"));

	/*Inicia o campo de entrada de texto */
	txtReg = GTK_WIDGET(gtk_builder_get_object(builder, "txtReg"));

	gtk_builder_connect_signals(builder, NULL);
	g_object_unref(builder);

	gtk_widget_show(janela1);

	gtk_main();

}
