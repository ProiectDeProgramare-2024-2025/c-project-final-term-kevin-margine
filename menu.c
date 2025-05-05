#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void set_color(int color_code) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color_code);
#endif
}

void reset_color() {
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
#endif
}


void clear_console() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


void header()
{

    printf("--------------\n");
    printf("1 - Admin\n");
    printf("2 - Customer\n");

    printf("--------------\n");
}

void admin_options() {


    printf("--------------\n");

    printf("1 - Add products to inventory\n");
    printf("2 - Delete products from inventory\n");
    printf("3 - View inventory\n");
    printf("4 - Adjust product stock\n");
    printf("5 - Change prices\n");

    printf("0 - Exit\n");
    printf("--------------\n");
}

void customer_options() {


    printf("--------------\n");

    printf("1 - Place order\n");

    printf("0 - Exit\n");
    printf("--------------\n");
}

void order_options() {


    printf("--------------\n");

    printf("1 - Add products to order\n");
    printf("2 - Remove products from order\n");
    printf("3 - View order\n");
    printf("4 - Submit order\n");

    printf("0 - Exit\n");
    printf("--------------\n");
}


void add_inventory(const char path[]) {
    FILE *filePointer = fopen(path, "a");
    if (filePointer == NULL) {
        printf("Error: The file cannot be accessed.\n");
        return;
    }

    char name[20];
    int quantity;
    float price;

    printf("--------------\n");
    printf("Add product to inventory\n");
    printf("--------------\n");

    printf("Enter name of the product (lower-case letters): ");
    scanf(" %[^\n]", name);

    printf("Enter product quantity (number): ");
    if (scanf("%d", &quantity) != 1) {
        printf("Invalid input for quantity.\n");
        fclose(filePointer);
        return;
    }

    printf("Enter product price (number): ");
    if (scanf("%f", &price) != 1) {
        printf("Invalid input for price.\n");
        fclose(filePointer);
        return;
    }

    fprintf(filePointer, "%s,%d,%.2f\n", name, quantity, price);
    fclose(filePointer);


    set_color(10);
    printf("Product ");
    set_color(11);
    printf("%s", name);
    set_color(10);
    printf(" with quantity ");
    set_color(14);
    printf("%d",quantity);
    set_color(10);
    printf(" has been added to the inventory with the price of ");
    set_color(13);
    printf("%.2f.\n",price);
    reset_color();
    printf("--------------\n");
}

void delete_inventory(const char path[]) {
    char name[20];

    printf("--------------\n");
    printf("Delete from inventory by name\n");
    printf("--------------\n");

    printf("Enter product name (lower case letters): ");
    scanf(" %[^\n]", name);

    FILE *filePointer = fopen(path, "r");
    if (filePointer == NULL) {
        set_color(12);
        printf("Error: The file cannot be accessed.\n");
        reset_color();
        return;
    }

    FILE *tempPointer = fopen("temp.txt", "w");
    if (tempPointer == NULL) {
        set_color(12);
        printf("Error: Cannot create a temporary file.\n");
        reset_color();
        fclose(filePointer);
        return;
    }

    char line[50], t_name[20];
    int t_quantity, found = 0;

    while (fgets(line, sizeof(line), filePointer)) {
        if (sscanf(line, "%[^,],%d", t_name, &t_quantity) == 2) {
            if (strcmp(name, t_name) != 0) {
                fprintf(tempPointer, "%s", line);
            } else {
                found = 1;
            }
        }
    }

    fclose(filePointer);
    fclose(tempPointer);

    if (found) {
        remove(path);
        rename("temp.txt", path);

        set_color(10);
        printf("Product ");
        set_color(11);
        printf("%s", name);
        set_color(10);
        printf(" deleted successfully.\n");
        reset_color();
    } else {
        set_color(12);
        printf("Product ");
        set_color(11);
        printf("%s", name);
        set_color(12);
        printf(" not found.\n");
        reset_color();
    }
    printf("--------------\n");
}

void view_inventory(const char path[]) {
    FILE *filePointer = fopen(path, "r");

    printf("--------------\n");
    printf("Inventory List:\n");
    printf("--------------\n");

    if (filePointer == NULL) {
        set_color(12);
        printf("Error: The file cannot be accessed.\n");
        reset_color();
        return;
    }

    char name[20];
    int quantity;
    float price;

    while (fscanf(filePointer, "%[^,],%d,%f\n", name, &quantity, &price) == 3) {
        set_color(10);
        printf("Product: ");

        set_color(11);
        printf("%s",name);

        set_color(10);
        printf(" | Quantity: ");

        set_color(14);
        printf("%d", quantity);

        set_color(10);
        printf(" | Price: ");

        set_color(13);
        printf("%.2f\n", price);

        reset_color();
    }

    fclose(filePointer);
    printf("--------------\n");
}




void adjust_stock(const char path[]) {
    char name[20];
    int adjustment;

    printf("--------------\n");
    printf("Adjust product stock\n");
    printf("--------------\n");

    printf("Enter product name (lower-case letters): ");
    scanf(" %[^\n]", name);

    printf("Enter stock adjustment amount (positive to add, negative to remove): ");
    if (scanf("%d", &adjustment) != 1) {
        set_color(12);
        printf("Invalid input for adjustment.\n");
        reset_color();
        return;
    }

    FILE *filePointer = fopen(path, "r");
    if (filePointer == NULL) {
        set_color(12);
        printf("Error: The file cannot be accessed.\n");
        reset_color();
        return;
    }

    FILE *tempPointer = fopen("temp.txt", "w");
    if (tempPointer == NULL) {
        set_color(12);
        printf("Error: Cannot create a temporary file.\n");
        reset_color();
        fclose(filePointer);
        return;
    }

    char line[50], t_name[20];
    int t_quantity;
    float t_price;
    int found = 0;

    while (fgets(line, sizeof(line), filePointer)) {
        if (sscanf(line, "%[^,],%d,%f", t_name, &t_quantity, &t_price) == 3) {
            if (strcmp(name, t_name) == 0) {
                t_quantity += adjustment;
                found = 1;
            }
            fprintf(tempPointer, "%s,%d,%.2f\n", t_name, t_quantity, t_price);
        }
    }

    fclose(filePointer);
    fclose(tempPointer);

    if (found) {
        remove(path);
        rename("temp.txt", path);
        set_color(10);
        printf("Product ");
        set_color(11);
        printf("%s", name);
        set_color(10);
        printf(" stock adjusted by ");
        set_color(14);
        printf("%d", adjustment);
        set_color(10);
        printf(".\n");
    } else {
        set_color(12);
        printf("Product ");
        set_color(11);
        printf("%s", name);
        set_color(12);
        printf(" not found.\n");
    }
    reset_color();
    printf("--------------\n");
}




void change_prices(const char path[]) {
    char name[20];
    float new_price;

    printf("--------------\n");
    printf("Change product price\n");
    printf("--------------\n");

    printf("Enter product name (lower-case letters): ");
    scanf(" %[^\n]", name);

    printf("Enter new price: ");
    if (scanf("%f", &new_price) != 1) {
        set_color(12);
        printf("Invalid input for price.\n");
        reset_color();
        return;
    }

    FILE *filePointer = fopen(path, "r");
    if (filePointer == NULL) {
        set_color(12);
        printf("Error: The file cannot be accessed.\n");
        reset_color();
        return;
    }

    FILE *tempPointer = fopen("temp.txt", "w");
    if (tempPointer == NULL) {
        set_color(12);
        printf("Error: Cannot create a temporary file.\n");
        reset_color();
        fclose(filePointer);
        return;
    }

    char line[50], t_name[20];
    int t_quantity;
    float t_price;
    int found = 0;

    while (fgets(line, sizeof(line), filePointer)) {
        if (sscanf(line, "%[^,],%d,%f", t_name, &t_quantity, &t_price) == 3) {
            if (strcmp(name, t_name) == 0) {
                t_price = new_price;
                found = 1;
            }
            fprintf(tempPointer, "%s,%d,%.2f\n", t_name, t_quantity, t_price);
        }
    }

    fclose(filePointer);
    fclose(tempPointer);

    if (found) {
        remove(path);
        rename("temp.txt", path);
        set_color(10);
        printf("Price of ");
        set_color(11);
        printf("%s", name);
        set_color(10);
        printf(" updated to ");
        set_color(13);
        printf("%.2f\n", new_price);
    } else {
        set_color(12);
        printf("Product ");
        set_color(11);
        printf("%s", name);
        set_color(12);
        printf(" not found.\n");
    }
    reset_color();
    printf("--------------\n");
}



void add_order(char path[])
{


    FILE *filePointer = fopen(path, "a");

    if (filePointer == NULL) {
        printf("Error: The file cannot be accessed.\n");
        return;
    }

    char name[20];
    int quantity;

    printf("--------------\n");
    printf("Add product to order\n");
    printf("--------------\n");

    printf("Enter name of the product (lower case letters): ");
    scanf(" %[^\n]", name);

    printf("Enter product quantity (number): ");
    if (scanf("%d", &quantity) != 1) {
        printf("Invalid input for quantity.\n");
        fclose(filePointer);
        return;
    }

    fprintf(filePointer, "%s,%d\n", name, quantity);
    fclose(filePointer);

    set_color(10);
    printf("Product ");
    set_color(11);
    printf("%s", name);
    set_color(10);
    printf(" with quantity ");
    set_color(14);
    printf("%d",quantity);
    set_color(10);
    printf(" has been added to the inventory \n");
    reset_color();
    printf("--------------\n");

}


void remove_order(char path[])
{


    char name[20];

    printf("--------------\n");
    printf("Delete from order by name\n");
    printf("--------------\n");

    printf("Enter product name (lower case letters): ");
    scanf(" %[^\n]", name);

    FILE *filePointer = fopen(path, "r");
    if (filePointer == NULL) {
        printf("Error: The file cannot be accessed.\n");
        return;
    }

    FILE *tempPointer = fopen("temp.txt", "w");
    if (tempPointer == NULL) {
        printf("Error: Cannot create a temporary file.\n");
        fclose(filePointer);
        return;
    }

    char line[50], t_name[20];
    int t_quantity, found = 0;

    while (fgets(line, sizeof(line), filePointer)) {
        if (sscanf(line, "%[^,],%d", t_name, &t_quantity) == 2) {
            if (strcmp(name, t_name) != 0) {
                fprintf(tempPointer, "%s", line);
            } else {
                found = 1;
            }
        }
    }

    fclose(filePointer);
    fclose(tempPointer);

    if (found) {
        remove(path);
        rename("temp.txt", path);

        set_color(10);
        printf("Product ");
        set_color(11);
        printf("%s", name);
        set_color(10);
        printf(" deleted successfully.\n");
        reset_color();
    } else {
        set_color(12);
        printf("Product ");
        set_color(11);
        printf("%s", name);
        set_color(12);
        printf(" not found.\n");
        reset_color();
    }
    printf("--------------\n");
}


void view_order(char order_path[], char inventory_path[])
{
    FILE *orderFile = fopen(order_path, "r");
    FILE *inventoryFile = fopen(inventory_path, "r");

    printf("--------------\n");
    printf("Order Summary:\n");
    printf("--------------\n");

    if (orderFile == NULL) {
        set_color(12); // Red for error
        printf("Error: The order file cannot be accessed.\n");
        reset_color();
        return;
    }

    if (inventoryFile == NULL) {
        set_color(12); // Red for error
        printf("Error: The inventory file cannot be accessed.\n");
        reset_color();
        return;
    }

    char order_name[50];
    int order_quantity;
    char inv_name[50];
    int inv_quantity;
    double inv_price;
    double total_price = 0.0;

    struct {
        char name[50];
        int quantity;
        double price;
    } inventory[100];
    int inv_count = 0;

    while (fgets(inv_name, sizeof(inv_name), inventoryFile)) {
        if (sscanf(inv_name, " %49[^,],%d,%lf", inventory[inv_count].name, &inventory[inv_count].quantity, &inventory[inv_count].price) == 3) {
            inventory[inv_count].name[strcspn(inventory[inv_count].name, "\r\n")] = 0;  // Trim newline
            inv_count++;
        }
    }
    fclose(inventoryFile);

    while (fscanf(orderFile, " %49[^,],%d", order_name, &order_quantity) == 2) {
        order_name[strcspn(order_name, "\r\n")] = 0;

        double product_price = 0.0;
        int found = 0;
        for (int i = 0; i < inv_count; i++) {
            if (strcmp(order_name, inventory[i].name) == 0) {
                product_price = inventory[i].price;
                found = 1;
                break;
            }
        }

        if (found) {
            set_color(10);
            printf("Product: ");

            set_color(11);
            printf("%s", order_name);

            set_color(10);
            printf(" | Quantity: ");

            set_color(14);
            printf("%d", order_quantity);

            set_color(10);
            printf(" | Price: $");

            set_color(13);
            printf("%.2f", product_price);

            double item_total = order_quantity * product_price;
            set_color(10);
            printf(" | Total: $");

            set_color(9);
            printf("%.2f", item_total);

            printf("\n");


            total_price += item_total;
        } else {
            set_color(12);
            printf("Error: Product \"%s\" not found in inventory.\n", order_name);
            reset_color();
        }
    }

    fclose(orderFile);

    printf("----------------------------\n");
    set_color(10);
    printf("Total Order Price: $");

    set_color(9);
    printf("%.2f\n", total_price);

    reset_color();
}






void submit_order(const char *order_path, const char *inventory_path)
{
    FILE *orderFile = fopen(order_path, "r");
    if (orderFile == NULL) {
        set_color(12); // Bright Red
        printf("Error: Could not access the order file.\n");
        reset_color();
        return;
    }

    char order_name[50];
    int order_quantity;
    struct {
        char name[50];
        int quantity;
    } orders[100];
    int order_count = 0;

    while (fscanf(orderFile, " %49[^,],%d", order_name, &order_quantity) == 2) {
        order_name[strcspn(order_name, "\r\n")] = 0;
        strcpy(orders[order_count].name, order_name);
        orders[order_count].quantity = order_quantity;
        order_count++;
    }
    fclose(orderFile);

    FILE *inventoryFile = fopen(inventory_path, "r+");
    if (inventoryFile == NULL) {
        set_color(12);
        printf("Error: Could not access the inventory file.\n");
        reset_color();
        return;
    }

    char inv_name[50];
    struct {
        char name[50];
        int quantity;
        double price;
    } inventory[100];
    int inv_count = 0;

    while (fgets(inv_name, sizeof(inv_name), inventoryFile)) {
        if (sscanf(inv_name, " %49[^,],%d,%lf", inventory[inv_count].name, &inventory[inv_count].quantity, &inventory[inv_count].price) == 3) {
            inventory[inv_count].name[strcspn(inventory[inv_count].name, "\r\n")] = 0;
            inv_count++;
        }
    }
    fclose(inventoryFile);

    double total = 0.0;

    set_color(15); // Bright White
    printf("Order Summary:\n");
    printf("----------------------------\n");
    reset_color();

    for (int i = 0; i < order_count; i++) {
        int found = 0;
        for (int j = 0; j < inv_count; j++) {
            if (strcmp(orders[i].name, inventory[j].name) == 0) {
                if (inventory[j].quantity >= orders[i].quantity) {
                    double item_total = orders[i].quantity * inventory[j].price;
                    total += item_total;
                    inventory[j].quantity -= orders[i].quantity;

                    set_color(14);
                    printf("%d", orders[i].quantity);
                    reset_color();

                    printf(" x ");

                    set_color(11);
                    printf("%s", orders[i].name);
                    reset_color();

                    printf(" @ ");

                    set_color(13);
                    printf("$%.2f", inventory[j].price);
                    reset_color();

                    printf(" = ");

                    set_color(9);
                    printf("$%.2f\n", item_total);
                    reset_color();
                } else {
                    set_color(12);
                    printf("Not enough stock for product: %s\n", orders[i].name);
                    reset_color();
                    return;
                }
                found = 1;
                break;
            }
        }
        if (!found) {
            set_color(12);
            printf("Product \"%s\" not found in inventory.\n", orders[i].name);
            reset_color();
            return;
        }
    }

    set_color(15);
    printf("----------------------------\n");
    printf("Total: ");
    set_color(9);
    printf("$%.2f\n", total);
    set_color(15);
    printf("----------------------------\n");
    reset_color();

    inventoryFile = fopen(inventory_path, "w");
    if (inventoryFile == NULL) {
        set_color(12);
        printf("Error: Could not write to inventory file.\n");
        reset_color();
        return;
    }
    for (int i = 0; i < inv_count; i++) {
        fprintf(inventoryFile, "%s,%d,%.2f\n", inventory[i].name, inventory[i].quantity, inventory[i].price);
    }
    fclose(inventoryFile);

    orderFile = fopen(order_path, "w");
    if (orderFile == NULL) {
        set_color(12);
        printf("Error: Could not clear order file.\n");
        reset_color();
        return;
    }
    fclose(orderFile);

    set_color(10);
    printf("Order submitted successfully!\n");
    reset_color();
}




void menu_admin(int op,char path[])
{
    switch(op)
    {
        case 1:
            add_inventory(path);
        break;
        case 2:
            delete_inventory(path);
        break;
        case 3:
            view_inventory(path);
        break;
        case 4:
            adjust_stock(path);
        break;
        case 5:
            change_prices(path);
        break;
        case 0:
            printf("Exiting admin menu...\n");
        break;
        default:
            printf("Invalid option!\n");
    }
}


void menu_order(char path[],const char inventory_path[])
{
    int order_option;
    do {
        order_options();
        printf("Please enter an option: ");
        scanf("%d", &order_option);
        switch(order_option) {
            case 1:
                add_order(path);
            break;
            case 2:
                remove_order(path);
            break;
            case 3:
                view_order(path,inventory_path);
            break;
            case 4:
                submit_order(path,inventory_path);
            break;
            case 0:
                printf("Exiting order menu...\n");
            break;
            default:
                printf("Invalid option!\n");
        }
    } while (order_option != 0);
}


void menu_customer(int op,char path[])
{
    switch(op)
    {
        case 1:
            menu_order(path,"admin.txt");
        break;
        case 0:
            printf("Exiting customer menu...\n");
        break;
        default:
            printf("Invalid option!\n");
    }
}


int main() {
    int option,user;
    char path1[]="admin.txt",path2[]="customer.txt";

    do {

        header();
        printf("Select user: ");
        scanf("%d", &user);

        if (user == 1) {
            do {

                admin_options();
                printf("Enter an option: ");
                scanf("%d", &option);
                menu_admin(option,path1);
            } while(option != 0);
        }
        else if (user == 2) {
            do {

                customer_options();
                printf("Enter an option: ");
                scanf("%d", &option);
                menu_customer(option,path2);
            } while(option != 0);
        }
        else if (user != 0) {
            printf("Invalid choice! Please select again.\n");
        }
    } while(user != 0);

    return 0;
}
