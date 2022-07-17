#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <gtk/gtk.h>

#define LENGTH 2048

// Global variables
GtkWidget *upperBox;
GtkWidget *label;
GtkWidget *label2;
GtkWidget *labelBox;
GtkWidget *labelBox2;

void send_msg_handler(const char *text);
//char message[LENGTH] = {};


volatile sig_atomic_t flag = 0;
int sockfd = 0;
char name[30];


static void load_css() {
	GtkCssProvider *provider;
	GdkDisplay *display;
	GdkScreen *screen;
	
	const gchar *css_style_file = "style.css";
	GFile *css_fp = g_file_new_for_path (css_style_file);
	GError *error = 0;
	
	provider = gtk_css_provider_new();
	display = gdk_display_get_default();
	screen = gdk_display_get_default_screen(display);
	
	gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	gtk_css_provider_load_from_file(provider, css_fp, &error);
	
	g_object_unref (provider);
}

static void sendMessage( GtkWidget *widget, gpointer data ){
	const char *text = gtk_entry_get_text(GTK_ENTRY(data));
	//g_print("%s", text);
	
	//GtkWidget *label;
	//label = gtk_label_new(text);
	//gtk_box_pack_start(GTK_BOX(upperBox), label, FALSE, TRUE, 0);
	
	gtk_label_set_label(GTK_LABEL(label), text);
	send_msg_handler(text);
}

static void receiveMessage(char *message){
	//g_print("%s", text);
//	for(int i=o; i<5; i++) {
	
//	}
	//GtkWidget *label;
	//label = gtk_label_new(text);
	//gtk_box_pack_start(GTK_BOX(upperBox), label, FALSE, TRUE, 0);
	
	gtk_label_set_label(GTK_LABEL(label2), message); 
}








void str_overwrite_stdout() {
  printf("%s", "> ");
  fflush(stdout);
}

void str_trim_lf (char* arr, int length) {
  int i;
  for (i = 0; i < length; i++) { // trim \n
    if (arr[i] == '\n') {
      arr[i] = '\0';
      break;
    }
  }
}

void catch_ctrl_c_and_exit(int sig) {
    flag = 1;
}

void send_msg_handler(const char *text) {
  char message[LENGTH] = {};
  char buffer[LENGTH + 32] = {};

  //while(1) {
  	str_overwrite_stdout();
  //  fgets(message, LENGTH, text);
    str_trim_lf(message, LENGTH);

	for(int i=0; i < LENGTH; i++) {
	message[i] = *text;
	text ++;
	
	}	
   // if (strcmp(message, "exit") == 0) {
	//		break;
   // } else {
      sprintf(buffer, "%s: %s\n", name, message);
      send(sockfd, buffer, strlen(buffer), 0);
  //  }

		bzero(message, LENGTH);
    bzero(buffer, LENGTH + 32);
 // }
  //catch_ctrl_c_and_exit(2);
}

void recv_msg_handler() {
	char *text;
	char message[LENGTH] = {};
	//char message2[LENGTH] = {};
  while (1) {
		int receive = recv(sockfd, message, LENGTH, 0);
    if (receive > 0) {
      //printf("%s", message);
      
      //for(int i=0; i < LENGTH; i++) {
	//message2[i] = message[i];
	//text++;
	
	//}
      text = &message[0];
      receiveMessage(text);
      str_overwrite_stdout();
    } else if (receive == 0) {
			break;
    } else {
			// -1
		}
		memset(message, 0, sizeof(message));
  }
}

int main(int argc, char **argv){
	if(argc != 2){
		printf("Usage: %s <port>\n", argv[0]);
		return EXIT_FAILURE;
	}

	char *ip = "10.7.85.69";
	int port = atoi(argv[1]);

	signal(SIGINT, catch_ctrl_c_and_exit);

	printf("Please enter your name: ");
  fgets(name, 32, stdin);
  str_trim_lf(name, strlen(name));


	if (strlen(name) > 32 || strlen(name) < 2){
		printf("Name must be less than 30 and more than 2 characters.\n");
		return EXIT_FAILURE;
	}

	struct sockaddr_in server_addr;

	/* Socket settings */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(ip);
  server_addr.sin_port = htons(port);


  // Connect to Server
  int err = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
  if (err == -1) {
		printf("ERROR: connect\n");
		return EXIT_FAILURE;
	}

	// Send name
	send(sockfd, name, 32, 0);

	printf("=== WELCOME TO THE CHATROOM ===\n");
	
	
	
	
	
	
	
	gtk_init(&argc, &argv);
	
	
	
	//Widgets
	GtkWidget *window;
	GtkWidget *mainBox;

	GtkWidget *lowerBox;
	GtkWidget *textEntry;
	GtkWidget *sendButton;
	GtkWidget *person;
	GtkWidget *user;


	
	load_css();
	
	
	//setting widgets
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL); //window
	mainBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); //box
	upperBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); //Messages Box
	lowerBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0); //Box containing Text Box and send
	textEntry = gtk_entry_new(); //textbox
	sendButton = gtk_button_new_with_label("Send Message"); //send button
	user = gtk_label_new(name);
	labelBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	label = gtk_label_new("Messages");
	person = gtk_label_new("Second Person");
	labelBox2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	label2 = gtk_label_new("Messages");
	
	//class names
	gtk_widget_set_name(upperBox, "upperBox");
	gtk_widget_set_name(lowerBox, "lowerBox");
	gtk_widget_set_name(textEntry, "textEntry");
	gtk_widget_set_name(sendButton, "button");
	gtk_widget_set_name(labelBox, "labelBox");
	gtk_widget_set_name(labelBox2, "labelBox");
	gtk_widget_set_name(label, "label");
	gtk_widget_set_name(label2, "label2");
	gtk_widget_set_name(user, "header");
	gtk_widget_set_name(person, "header");
	
	//window setters
	gtk_window_set_title(GTK_WINDOW(window), "TCP Chat Application"); //header
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER); //centering window
	
	//container
	gtk_container_add(GTK_CONTAINER(window), mainBox); //container inside window
	//gtk_container_add(GTK_CONTAINER(mainBox), upperBox);
	//gtk_container_add(GTK_CONTAINER(mainBox), lowerBox);
	
	//packing
	gtk_box_pack_start(GTK_BOX(mainBox), upperBox, TRUE, TRUE, 10);
	gtk_widget_set_size_request(GTK_WIDGET(upperBox), 300, 400);
	
	gtk_box_pack_start(GTK_BOX(upperBox), user, FALSE, TRUE, 0);
	
	gtk_box_pack_start(GTK_BOX(upperBox), labelBox, TRUE, TRUE, 10);
	gtk_widget_set_size_request(GTK_WIDGET(labelBox), 300, 30);
	
	gtk_box_pack_start(GTK_BOX(upperBox), person, FALSE, TRUE, 0);
	
	gtk_box_pack_start(GTK_BOX(upperBox), labelBox2, TRUE, TRUE, 10);
	gtk_widget_set_size_request(GTK_WIDGET(labelBox), 300, 30);
	
	gtk_box_pack_start(GTK_BOX(mainBox), lowerBox, TRUE, TRUE, 10);
	gtk_widget_set_size_request(GTK_WIDGET(lowerBox), 300, 50);
	
	gtk_box_pack_start(GTK_BOX(lowerBox), textEntry, FALSE, TRUE, 0);
	gtk_widget_set_size_request(GTK_WIDGET(textEntry), 200, 50);
	
	gtk_box_pack_start(GTK_BOX(lowerBox), sendButton, FALSE, TRUE, 0);
	gtk_widget_set_size_request(GTK_WIDGET(sendButton), 100, 50);
	
	gtk_box_pack_start(GTK_BOX(labelBox), label, FALSE, TRUE, 0);
	gtk_label_set_xalign(GTK_LABEL(label),  0);
	
	gtk_box_pack_start(GTK_BOX(labelBox2), label2, FALSE, TRUE, 0);
	gtk_label_set_xalign(GTK_LABEL(label2),  0);

	
	//event handler
	g_signal_connect(sendButton, "clicked", G_CALLBACK(sendMessage), textEntry);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL); //killing application upon presseing cross
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

	pthread_t send_msg_thread;
  //if(pthread_create(&send_msg_thread, NULL, (void *) send_msg_handler, NULL) != 0){
		//printf("ERROR: pthread\n");
    //return EXIT_FAILURE;
	//}

	pthread_t recv_msg_thread;
  if(pthread_create(&recv_msg_thread, NULL, (void *) recv_msg_handler, NULL) != 0){
		printf("ERROR: pthread\n");
		return EXIT_FAILURE;
	}
	
	gtk_widget_show_all(window); //showing all the widgets
	
	
	
	gtk_main();

	while (1){
		if(flag){
			printf("\nBye\n");
			break;
    }
	}

	close(sockfd);
	
	

	return EXIT_SUCCESS;
}
