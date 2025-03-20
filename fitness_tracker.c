#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Structure to hold user details
struct User {
    char name[50];
    int age;
    float weight;
    float height;
    char fitnessGoal[30];
    char gender[10];
};

// Structure to hold daily progress data
struct Progress {
    int daysLogged;
    float caloriesBurned[30];
    int workoutsCompleted[30];
    float weightLog[30];
};

// Function prototypes
void displayWelcomeMessage();
void createProfile(struct User *user);
void saveUserData(struct User user);
void loadUserData(struct User *user, struct Progress *progress);
void generateWorkoutPlan(struct User user);
void displayWorkoutDetails(const char *goal);
void trackDailyProgress(struct Progress *progress, struct User user);
void displayProgressSummary(struct Progress progress);
void suggestAdjustments(struct User user, struct Progress progress);
void suggestDietPlan(struct User user);
float calculateBMI(float weight, float height);
int detectAutomation();

// Main function
int main() {
    struct User user;
    struct Progress progress = {0}; // Initialize progress tracking

    // Display welcome message
    displayWelcomeMessage();

    // Step 1: User Profile Creation or Load Existing
    loadUserData(&user, &progress);

    // Main menu loop
    int choice;
    do {
        if (detectAutomation()) {
            printf("\033[1;31mAutomation detected. Please use the program manually.\033[0m\n");
            exit(1);
        }

        printf("\033[1;34m\n=== Personalized Fitness Assistant ===\033[0m\n");
        printf("1. View Workout Plan\n");
        printf("2. Log Daily Progress\n");
        printf("3. View Progress Summary\n");
        printf("4. Get Suggestions for Improvement\n");
        printf("5. View Personalized Diet Plan\n");
        printf("6. Exit\n");
        printf("\033[1;33mEnter your choice: \033[0m");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                generateWorkoutPlan(user);
                break;
            case 2:
                trackDailyProgress(&progress, user);
                break;
            case 3:
                displayProgressSummary(progress);
                break;
            case 4:
                suggestAdjustments(user, progress);
                break;
            case 5:
                suggestDietPlan(user);
                break;
            case 6:
                printf("\033[1;32mThank you for using the Fitness Assistant! Stay healthy!\033[0m\n");
                saveUserData(user);
                break;
            default:
                printf("\033[1;31mInvalid choice! Please select again.\033[0m\n");
        }
    } while (choice != 6);

    return 0;
}

// Function Definitions

// Welcome message
void displayWelcomeMessage() {
    printf("\033[1;32m===============================================\n");
    printf("=Welcome to the Personalized Fitness Assistant=\n");
    printf("===============================================\033[0m\n");
    printf("This program helps you achieve your fitness goals\n");
    printf("by providing personalized workout plans, tracking your\n");
    printf("progress, and suggesting improvements.\n");
    printf("\nLet's get started!\n");
}

// Create or load user profile
void createProfile(struct User *user) {
    printf("\n\033[1;33m--- Create Your Profile ---\033[0m\n");
    printf("Enter your name: ");
    scanf(" %[^\n]", user->name);
    printf("Enter your gender (Male/Female/Other): ");
    scanf("%s", user->gender);
    printf("Enter your age: ");
    scanf("%d", &user->age);
    printf("Enter your weight (kg): ");
    scanf("%f", &user->weight);
    printf("Enter your height (m): ");
    scanf("%f", &user->height);
    printf("Enter your fitness goal (Weight Loss / Muscle Gain / General Fitness): ");
    scanf(" %[^\n]", user->fitnessGoal);

    printf("\033[1;34m\nProfile created successfully! Welcome, %s.\033[0m\n", user->name);
    float bmi = calculateBMI(user->weight, user->height);
    printf("\033[1;33mYour current BMI is: %.2f\033[0m\n", bmi);
    saveUserData(*user);
}

// Save user data to a file
void saveUserData(struct User user) {
    char filename[60];
    snprintf(filename, sizeof(filename), "%s_profile.txt", user.name);
    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        printf("\033[1;31mError saving user data.\033[0m\n");
        return;
    }

    fprintf(file, "%s\n%d\n%.2f\n%.2f\n%s\n%s\n", user.name, user.age, user.weight, user.height, user.fitnessGoal, user.gender);
    fclose(file);
    printf("\033[1;32mUser data saved successfully!\033[0m\n");
}

// Load user data from a file
void loadUserData(struct User *user, struct Progress *progress) {
    printf("\033[1;33mDo you want to load an existing profile? (1 for Yes, 0 for No): \033[0m");
    int loadProfile;
    scanf("%d", &loadProfile);

    if (loadProfile) {
        char filename[60];
        printf("Enter your name to load your profile: ");
        scanf(" %[^\n]", user->name);
        snprintf(filename, sizeof(filename), "%s_profile.txt", user->name);

        FILE *file = fopen(filename, "r");
        if (file == NULL) {
            printf("\033[1;31mNo profile found. Creating a new one.\033[0m\n");
            createProfile(user);
        } else {
            fscanf(file, "%[^\n]\n%d\n%f\n%f\n%[^\n]\n%[^\n]", user->name, &user->age, &user->weight, &user->height, user->fitnessGoal, user->gender);
            fclose(file);
            printf("\033[1;32mProfile loaded successfully!\033[0m\n");
        }
    } else {
        createProfile(user);
    }
}

// Detect automation (basic check)
int detectAutomation() {
    static time_t lastCall = 0;
    time_t currentTime = time(NULL);

    if (lastCall != 0 && (currentTime - lastCall < 2)) {
        return 1; // Automation detected
    }

    lastCall = currentTime;
    return 0; // No automation detected
}

// Generate workout plan
void generateWorkoutPlan(struct User user) {
    printf("\033[1;34m\n--- Personalized Workout Plan ---\033[0m\n");

    if (strcmp(user.fitnessGoal, "Weight Loss") == 0) {
        printf("Cardio (4 days/week):\n  - Running: 30 mins\n  - Jumping Jacks: 3 sets of 15 reps\n");
        printf("Strength (2 days/week):\n  - Push-ups: 3 sets of 12 reps\n  - Squats: 3 sets of 15 reps\n");
    } else if (strcmp(user.fitnessGoal, "Muscle Gain") == 0) {
        printf("Strength (4 days/week):\n  - Bench Press: 4 sets of 10 reps\n  - Deadlifts: 4 sets of 8 reps\n");
        printf("Cardio (1 day/week):\n  - Cycling: 30 mins\n");
    } else {
        printf("General Fitness Plan:\n  - Yoga: 30 mins\n  - Light Jogging: 20 mins\n  - Planks: 3 sets of 30 seconds\n");
    }
}

// Suggest diet plan
void suggestDietPlan(struct User user) {
    printf("\033[1;34m\n--- Personalized Diet Plan ---\033[0m\n");

    if (strcmp(user.fitnessGoal, "Weight Loss") == 0) {
        printf("- Breakfast: Poha / Upma with a boiled egg or sprouts\n");
        printf("- Lunch: 1 bowl dal, 1 roti, sabzi, and a bowl of curd\n");
        printf("- Dinner: Vegetable soup with a small serving of brown rice\n");
    } else if (strcmp(user.fitnessGoal, "Muscle Gain") == 0) {
        printf("- Breakfast: Paratha with curd / Peanut butter on whole wheat bread + boiled eggs\n");
        printf("- Lunch: Chicken curry / Paneer bhurji with rice and dal\n");
        printf("- Dinner: Dal khichdi with ghee and salad\n");
    } else {
        printf("- Balanced Diet:\n");
        printf("  - Breakfast: Dalia with milk or besan cheela with chutney\n");
        printf("  - Lunch: Roti, dal, sabzi, and buttermilk\n");
        printf("  - Dinner: Light khichdi with salad or vegetable curry with roti\n");
    }
}


// Track daily progress
void trackDailyProgress(struct Progress *progress, struct User user) {
    if (progress->daysLogged >= 30) {
        printf("\033[1;31mProgress log is full. Resetting...\033[0m\n");
        progress->daysLogged = 0;
    }

    printf("\n\033[1;33m--- Log Today's Progress ---\033[0m\n");
    printf("Calories burned: ");
    scanf("%f", &progress->caloriesBurned[progress->daysLogged]);
    printf("Number of workouts completed: ");
    scanf("%d", &progress->workoutsCompleted[progress->daysLogged]);
    printf("Current weight (kg): ");
    scanf("%f", &progress->weightLog[progress->daysLogged]);
    progress->daysLogged++;

    printf("\033[1;32mProgress logged successfully!\033[0m\n");
}

// Display progress summary
void displayProgressSummary(struct Progress progress) {
    if (progress.daysLogged == 0) {
        printf("\033[1;31mNo progress data logged yet.\033[0m\n");
        return;
    }

    printf("\n\033[1;34m--- Progress Summary ---\033[0m\n");
    printf("Days logged: %d\n", progress.daysLogged);
    float totalCalories = 0, totalWeightChange = 0;
    int totalWorkouts = 0;

    for (int i = 0; i < progress.daysLogged; i++) {
        totalCalories += progress.caloriesBurned[i];
        totalWorkouts += progress.workoutsCompleted[i];
        if (i > 0) {
            totalWeightChange += progress.weightLog[i] - progress.weightLog[i - 1];
        }
    }

    printf("Average calories burned: %.2f\n", totalCalories / progress.daysLogged);
    printf("Total workouts completed: %d\n", totalWorkouts);
    printf("Weight change over the period: %.2f kg\n", totalWeightChange);
}

// Suggest adjustments based on progress
void suggestAdjustments(struct User user, struct Progress progress) {
    printf("\n\033[1;34m--- Suggestions for Improvement ---\033[0m\n");

    if (progress.daysLogged < 7) {
        printf("Not enough data to make suggestions. Log at least a week's data.\n");
        return;
    }

    float avgCalories = 0;
    for (int i = 0; i < progress.daysLogged; i++) {
        avgCalories += progress.caloriesBurned[i];
    }
    avgCalories /= progress.daysLogged;

    if (strcmp(user.fitnessGoal, "Weight Loss") == 0 && avgCalories < 500) {
        printf("- Increase cardio intensity to burn more calories.\n");
    } else if (strcmp(user.fitnessGoal, "Muscle Gain") == 0 && avgCalories > 700) {
        printf("- Reduce calorie burn and focus more on strength training.\n");
    } else {
        printf("- Maintain current activity levels and ensure consistency.\n");
    }
}

// Calculate BMI
float calculateBMI(float weight, float height) {
    return weight / (height * height);
}
