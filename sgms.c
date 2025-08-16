#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Product {
    int code;
    char name[50];
    float price;
    int stock;
};

struct CartItem {
    int code;
    char name[50];
    int qty;
    float price;
    struct CartItem *next;
};

struct Bill {
    int custId;
    char name[50];
    float total;
};


struct Product products[] = {
    {101, "Rice", 50, 100},
    {102, "Oil", 120, 50},
    {103, "Sugar", 70, 80},
    {104, "Milk", 60, 40},
    {105, "Bread", 40, 60}
};
int productCount = 5;

struct CartItem *cart = NULL;
struct Bill dailyBills[50];
int top = -1;


void showProducts() {
    printf("\n Product List \n");
    for (int i = 0; i < productCount; i++) {
        printf("%d - %s (%.2f Tk) Stock: %d\n",
               products[i].code, products[i].name, products[i].price, products[i].stock);
    }
}

void addToCart(int code, int qty) {
    for (int i = 0; i < productCount; i++) {
        if (products[i].code == code && products[i].stock >= qty) {
            struct CartItem *newNode = malloc(sizeof(struct CartItem));
            newNode->code = code;
            strcpy(newNode->name, products[i].name);
            newNode->qty = qty;
            newNode->price = products[i].price * qty;
            newNode->next = cart;
            cart = newNode;

            products[i].stock -= qty;
            printf("Added %d x %s to cart.\n", qty, products[i].name);
            return;
        }
    }
    printf("Product not found or insufficient stock.\n");
}

void showCart() {
    if (cart == NULL) {
        printf("Cart is empty.\n");
        return;
    }
    printf("\n--- Shopping Cart ---\n");
    struct CartItem *t = cart;
    float sum = 0;
    while (t != NULL) {
        printf("%d - %s x%d = %.2f\n", t->code, t->name, t->qty, t->price);
        sum += t->price;
        t = t->next;
    }
    printf("Cart Total = %.2f\n", sum);
}

void checkout(int id, char name[]) {
    if (cart == NULL) {
        printf("Cart is empty.\n");
        return;
    }
    struct CartItem *t = cart;
    float total = 0;
    while (t != NULL) {
        total += t->price;
        t = t->next;
    }
    float discount = (total > 1000) ? total * 0.1 : 0;
    float final = total - discount;

    printf("\n Bill for %s (ID:%d) \n", name, id);
    printf("Total = %.2f\n", total);
    printf("Discount = %.2f\n", discount);
    printf("Payable = %.2f\n", final);


    dailyBills[++top].custId = id;
    strcpy(dailyBills[top].name, name);
    dailyBills[top].total = final;


    while (cart != NULL) {
        struct CartItem *temp = cart;
        cart = cart->next;
        free(temp);
    }
}

void dailyReport() {
    if (top == -1) {
        printf("No sales today.\n");
        return;
    }
    printf("\n--- Daily Report ---\n");
    float sum = 0;
    float maxSale = 0;
    char topCust[50];
    for (int i = 0; i <= top; i++) {
        printf("%d - %s : %.2f\n", dailyBills[i].custId, dailyBills[i].name, dailyBills[i].total);
        sum += dailyBills[i].total;
        if (dailyBills[i].total > maxSale) {
            maxSale = dailyBills[i].total;
            strcpy(topCust, dailyBills[i].name);
        }
    }
    printf("Total Sales: %.2f\n", sum);
    printf("Top Customer: %s (%.2f)\n", topCust, maxSale);

    printf("\n Remaining Stock \n");
    for (int i = 0; i < productCount; i++) {
        printf("%d - %s : %d left\n", products[i].code, products[i].name, products[i].stock);
    }
}


int main() {
    int choice, code, qty, id;
    char name[50];

    while (1) {
        printf("\n--- Grocery Menu ---\n");
        printf("1. Show Products\n");
        printf("2. Add to Cart\n");
        printf("3. Show Cart\n");
        printf("4. Checkout\n");
        printf("5. Daily Report\n");
        printf("6. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                showProducts();
                break;
            case 2:
                printf("Enter Product Code: "); scanf("%d", &code);
                printf("Enter Quantity: "); scanf("%d", &qty);
                addToCart(code, qty);
                break;
            case 3:
                showCart();
                break;
            case 4:
                printf("Enter Customer ID: "); scanf("%d", &id);
                printf("Enter Customer Name: "); scanf(" %[^\n]", name);
                checkout(id, name);
                break;
            case 5:
                dailyReport();
                break;
            case 6:
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }
}
