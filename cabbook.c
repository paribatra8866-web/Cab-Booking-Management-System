#include <stdio.h>

#define MAX_CABS 10
#define MAX_REQUESTS 10
#define MAX_TRIPS 10

struct Cab
{
    int id;
    char driver[30];
    float farePerKm;
    int available;
};

struct RideRequest
{
    int customerId;
    char pickup[30];
    char destination[30];
    float distance;
};

struct Trip
{
    int customerId;
    int cabId;
    float fare;
};

struct Cab cabs[MAX_CABS] =
{
    {101, "Aman", 12.0, 1},
    {102, "Riya", 10.0, 1},
    {103, "Karan", 15.0, 1},
    {104, "Neha", 11.0, 1}
};

int cabCount = 4;

struct RideRequest requests[MAX_REQUESTS];
int front = -1;
int rear = -1;

struct Trip trips[MAX_TRIPS];
int top = -1;

void displayCabs()
{
    int i;

    printf("\nCab List\n");
    printf("ID\tDriver\tFare/km\tStatus\n");

    for (i = 0; i < cabCount; i++)
    {
        printf("%d\t%s\t%.2f\t",
               cabs[i].id,
               cabs[i].driver,
               cabs[i].farePerKm);

        if (cabs[i].available == 1)
        {
            printf("Available\n");
        }
        else
        {
            printf("On trip\n");
        }
    }
}

void searchCab()
{
    int id;
    int i;
    int found = 0;

    printf("Enter cab ID to search: ");
    scanf("%d", &id);

    for (i = 0; i < cabCount; i++)
    {
        if (cabs[i].id == id)
        {
            printf("Cab found\n");
            printf("Driver: %s\n", cabs[i].driver);
            printf("Fare per km: %.2f\n", cabs[i].farePerKm);

            if (cabs[i].available == 1)
            {
                printf("Status: Available\n");
            }
            else
            {
                printf("Status: On trip\n");
            }

            found = 1;
            break;
        }
    }

    if (found == 0)
    {
        printf("Cab not found.\n");
    }
}

void sortCabsByFare()
{
    int i;
    int j;
    struct Cab temp;

    for (i = 0; i < cabCount - 1; i++)
    {
        for (j = 0; j < cabCount - i - 1; j++)
        {
            if (cabs[j].farePerKm > cabs[j + 1].farePerKm)
            {
                temp = cabs[j];
                cabs[j] = cabs[j + 1];
                cabs[j + 1] = temp;
            }
        }
    }

    printf("Cabs sorted by fare, lowest first.\n");
    displayCabs();
}

int isQueueFull()
{
    if ((rear + 1) % MAX_REQUESTS == front)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int isQueueEmpty()
{
    if (front == -1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void addRideRequest()
{
    struct RideRequest request;

    if (isQueueFull())
    {
        printf("The ride request queue is full.\n");
        return;
    }

    printf("Enter customer ID: ");
    scanf("%d", &request.customerId);

    printf("Enter pickup location: ");
    scanf("%s", request.pickup);

    printf("Enter destination: ");
    scanf("%s", request.destination);

    printf("Enter distance in kilometres: ");
    scanf("%f", &request.distance);

    if (request.distance <= 0)
    {
        printf("Distance must be greater than zero.\n");
        return;
    }

    if (isQueueEmpty())
    {
        front = 0;
    }

    rear = (rear + 1) % MAX_REQUESTS;
    requests[rear] = request;

    printf("Ride request added to the queue.\n");
}

void processRideRequest()
{
    int cabIndex = -1;
    int i;
    float fare;

    struct RideRequest request;

    if (isQueueEmpty())
    {
        printf("There are no ride requests to process.\n");
        return;
    }

    if (top == MAX_TRIPS - 1)
    {
        printf("Trip history is full.\n");
        return;
    }

    for (i = 0; i < cabCount; i++)
    {
        if (cabs[i].available == 1)
        {
            cabIndex = i;
            break;
        }
    }

    if (cabIndex == -1)
    {
        printf("No cabs are currently available.\n");
        return;
    }

    request = requests[front];

    if (front == rear)
    {
        front = -1;
        rear = -1;
    }
    else
    {
        front = (front + 1) % MAX_REQUESTS;
    }

    fare = request.distance * cabs[cabIndex].farePerKm;

    cabs[cabIndex].available = 0;

    top++;

    trips[top].customerId = request.customerId;
    trips[top].cabId = cabs[cabIndex].id;
    trips[top].fare = fare;

    printf("\nRide assigned successfully.\n");
    printf("Customer ID: %d\n", request.customerId);
    printf("Route: %s to %s\n", request.pickup, request.destination);
    printf("Cab ID: %d\n", cabs[cabIndex].id);
    printf("Driver: %s\n", cabs[cabIndex].driver);
    printf("Fare: %.2f\n", fare);
}

void displayTripHistory()
{
    int i;

    if (top == -1)
    {
        printf("There are no completed trips.\n");
        return;
    }

    printf("\nTrip History\n");

    for (i = top; i >= 0; i--)
    {
        printf("Customer: %d | Cab: %d | Fare: %.2f\n",
               trips[i].customerId,
               trips[i].cabId,
               trips[i].fare);
    }
}

void undoLastTrip()
{
    int i;
    struct Trip lastTrip;

    if (top == -1)
    {
        printf("There is no trip to undo.\n");
        return;
    }

    lastTrip = trips[top];

    top--;

    for (i = 0; i < cabCount; i++)
    {
        if (cabs[i].id == lastTrip.cabId)
        {
            cabs[i].available = 1;
            break;
        }
    }

    printf("Last trip for customer %d was removed.\n",
           lastTrip.customerId);

    printf("Cab %d is available again.\n",
           lastTrip.cabId);
}

int main()
{
    int choice;

    do
    {
        printf("\nCab Booking Management System\n");
        printf("1. Display cabs\n");
        printf("2. Search for a cab\n");
        printf("3. Sort cabs by fare\n");
        printf("4. Add a ride request\n");
        printf("5. Process the next ride request\n");
        printf("6. Display trip history\n");
        printf("7. Undo the last trip\n");
        printf("0. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                displayCabs();
                break;

            case 2:
                searchCab();
                break;

            case 3:
                sortCabsByFare();
                break;

            case 4:
                addRideRequest();
                break;

            case 5:
                processRideRequest();
                break;

            case 6:
                displayTripHistory();
                break;

            case 7:
                undoLastTrip();
                break;

            case 0:
                printf("Exiting the program.\n");
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 0);

    return 0;
}

