//
//  main.c
//  QR_algorithm
//
//  Created by Luke Poeppel on 5/8/19.
//  Copyright © 2019 Luke Poeppel. All rights reserved.
//
/*
 Program for approximating the eigenvalues of a 3x3 matrix by the QR method. This method requires vector operations (subtraction, dot product, Gram-Schmidt orthonormalization, etc.) and matrix operations (product, powers). This program queries the user for a 3x3 matrix - entered one row at a time - and returns the approximated eigenvalues after 50 iterations. Note that if the user inputs a matrix with linearly dependant columns, incorrect results will be returned; this algorithm is quite finnicky since it depends upon the properties of the matrix; some symmetric matrices, for example, do not always converge absolutely to their eigenvalue.
 
 QR decomposition: We find the Q matrix of the QR decomposition by the Gram-Schmidt procedure. The R is found by "book-keeping," in the same manner as the LU decomposition keeping track of multipliers in Gaussian elimination; thus R is, albeit approximately, an upper triangular matrix. By the properties of orthogonal matrices, R is found simply by multiplying the transpose of Q with the original matrix A.
 */
#include <stdio.h>                                                                          // Include standard libraries.
#include <stdlib.h>
#include <math.h>

#define LIMIT 100                                                                            // Set the limit of our calculations to 50 steps.

// Function prototypes
float * vector_subtraction(float vectorA[], float vectorB[]);                               // Function for vector subtraction.
float vector_dot_product(float vectorA[], float vectorB[]);                                 // Function for vector dot products.
float * vector_scalar_product(float alpha, float vector[]);                                 // Function for vector-scalar products.
float get_norm(float vector[]);                                                             // Function to find the norm of a vector.
float * normalize(float vector[]);                                                          // Function to normalize a vector.
void print_vector(float vector[3]);                                                         // Unused function for vector printing.

void matrix_product(float matrixA[][3], float matrixB[][3], float out[][3]);                // Function for square matrix multiplication.
void print_matrix(float matrix[][3]);                                                       // Function for matrix printing.
void tranpose(float matrix[][3]);                                                           // Function for in place transposition of matrices.
float * get_matrix_column(float matrix[][3], int column);                                   // Function for retrieving a queried matrix column.

float * QR_Decomposition(float matrix[][3], float q_out[][3], float r_out[][3]);            // Function for finding the QR decomposition of a matrix.
float * QR_Algo(float matrix[][3], int stepLimit, float eigenvalues[]);                     // Function for finding an *array* of approximate eigenvalues.

int main() {
    float matrix[3][3];                                                                     // Initialize 2D matrix array.

    printf("-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-\n");
    printf("Enter Row 1 (elements separated): ");                                           // Matrix (A) population.
    for (int i = 0; i < 3; i++) {
        scanf("%f", &matrix[0][i]);
    }
    
    printf("Enter Row 2 (elements separated): ");
    for (int j = 0; j < 3; j++) {
        scanf("%f", &matrix[1][j]);
    }
    
    printf("Enter Row 3 (elements separated): ");
    for (int k = 0; k < 3; k++) {
        scanf("%f", &matrix[2][k]);
    }
    
    printf("-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-\n");
    printf("ORIGINAL MATRIX:\n");
    print_matrix(matrix);                                                                   // Print original matrix.
    printf("\n");
    
    printf("-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-\n");
    printf("INITIAL QR-DECOMPOSITION: \n\nQ = ");
    tranpose(matrix);                                                                       // Transpose the matrix so as to perform Gram-Schmidt.
    
    float q_out[3][3];                                                                      // Arrays for storing decomposition data.
    float r_out[3][3];
    QR_Decomposition(matrix, q_out, r_out);                                                 // Find and print the QR decomposition of the input matrix.
    print_matrix(q_out);
    printf("\n");
    printf("\nR = (Q^T)A = ");
    print_matrix(r_out);
    
    printf("\n-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-\n");
    printf("After %d iterations, we approximate the eigenvalues of A to be: \n", LIMIT);
    
    float eigenvalues[3];                                                                   // Array for storing approximated eigenvalues.
    QR_Algo(matrix, LIMIT, eigenvalues);                                                    // Generate approximate eigenvalues of A and store them in array.
    
    for (int i = 0; i < 3; i++) {                                                           // Print all approximated eigenvalues.
        printf("lambda%d = %.6f\n", i, eigenvalues[i]);
    }
    
    return 0;
}

// Given two vectors A and B, subtracts A from B.
float * vector_subtraction(float vectorA[], float vectorB[]) {
    float *subtracted_vector = malloc(4 * sizeof(subtracted_vector));
    for (int i = 0; i < 3; i++) {
        subtracted_vector[i] = vectorA[i] - vectorB[i];                                     // Subtract the vectors component by component.
    }

    return subtracted_vector;
}

// Given two vectors A and B, returns their dot product.
float vector_dot_product(float vectorA[], float vectorB[]) {
    float sum = 0;
    for (int i = 0; i < 3; i++) {
        sum += (vectorA[i] * vectorB[i]);                                                   // Multiply the vectors component by component and add to sum.
    }
    
    return sum;
}

// Given a scalar and a vector, returns a new vector with each component multiplied by the scalar.
float * vector_scalar_product(float alpha, float vector[]) {
    float *scaled_vector = malloc(4 * sizeof(scaled_vector));                               // Dynamically allocate space for scaled vector.
    for (int i = 0; i < 3; i++) {
        scaled_vector[i] = (alpha * vector[i]);                                             // Multiply the vector by the scalar component by component.
    }
    
    return scaled_vector;
}

// Given a vector, returns its Euclidian norm.
float get_norm(float vector[]) {
    float norm = sqrt(vector_dot_product(vector, vector));                                  // Standard definition of Euclidian norm.
    return norm;
}

// Given a vector, returns a new vector with normalized components.
float * normalize(float vector[]) {
    float *normalized_vector = malloc(4 * sizeof(*normalized_vector));                      // Dynamically allocate space for normalized vector.
    float norm = get_norm(vector);                                                          // Compute the norm of the input vector.
    for (int i = 0; i < 3; i++) {
        float normalized_value = (vector[i] / norm);
        normalized_vector[i] = normalized_value;
    }
    
    return normalized_vector;
}

// Prints a matrix.
void print_matrix(float matrix[][3]) {
    for (int i = 0; i < 3; i++) {
        printf("\n");
        for (int j = 0; j < 3; j++) {
            printf("%.6f ", matrix[i][j]);
        }
    }
}

// Prints a vector (this was very useful for debugging but is not used anywhere in the code.)
void print_vector(float vector[3]) {
    for (int i = 0; i < 3; i++) {
        printf("%.6f\n", vector[i]);
    }
}

// Given two matrices A and B, returns their product.
void matrix_product(float matrixA[][3], float matrixB[][3], float out[][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            out[i][j] = 0;
            for (int k = 0; k < 3; k++) {
                float v = (matrixA[i][k] * matrixB[k][j]);                                  // Multiply component by component (row/column).
                out[i][j] += v;
            }
        }
    }
}

// Transposes a given *square matrix*
void tranpose(float matrix[][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = (i+1); j < 3; j++) {
            float temp = matrix[i][j];                                                      // Swap the components.
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = temp;
        }
    }
}

// C has trouble with returning arrays within multidimensional (2D) arrays. This function accepts a matrix and a column number and returns an array with the elements of the matrix at that column.
float * get_matrix_column(float matrix[][3], int column) {
    float *column_vector = malloc(4 * sizeof(column));                                      // Dynamically allocate memory for vector to be returned.
    for (int i = 0; i < 3; i++) {
        column_vector[i] = matrix[column][i];                                               // Assign each element to the vector.
    }

    return column_vector;
}

// Given a matrix, returns the QR decomposition of that matrix.
float * QR_Decomposition(float matrix[][3], float q_out[][3], float r_out[][3]) {
    // First vector
    float *q1, q1_norm_squared;
    q1 = get_matrix_column(matrix, 0);
    q1_norm_squared = get_norm(q1) * get_norm(q1);
    
    // Second vector
    float *v2, v2_dotA, v2_projection_coefficient, *v2_projection, *q2;                     // God help us... (just follow the equation).
    v2 = get_matrix_column(matrix, 1);
    v2_dotA = vector_dot_product(v2, q1);
    v2_projection_coefficient = (v2_dotA / q1_norm_squared);
    v2_projection = vector_scalar_product(v2_projection_coefficient, q1);
    q2 = vector_subtraction(v2, v2_projection);
    
    // Third vector
    float *v3, v3_projection_A_dot1, v3_projection_A_coefficient, *v3_projection_A;
    float v3_projection_B_dot1, q2_norm_squared, v3_projection_B_coefficient, *v3_projection_B, *q3_subtract_a, *q3;
    v3 = get_matrix_column(matrix, 2);
    
    // PROJECTION A
    v3_projection_A_dot1 = vector_dot_product(v3, q1);
    v3_projection_A_coefficient = (v3_projection_A_dot1 / q1_norm_squared);
    v3_projection_A = vector_scalar_product(v3_projection_A_coefficient, q1);

    // PROJECTION B
    v3_projection_B_dot1 = vector_dot_product(v3, q2);
    q2_norm_squared = get_norm(q2) * get_norm(q2);
    v3_projection_B_coefficient = (v3_projection_B_dot1 / q2_norm_squared);
    v3_projection_B = vector_scalar_product(v3_projection_B_coefficient, q2);
    q3_subtract_a = vector_subtraction(v3, v3_projection_A);
    q3 = vector_subtraction(q3_subtract_a, v3_projection_B);
    
    // NORMALIZE
    float *q1_normalized, *q2_normalized, *q3_normalized;                                   // Normalize each vector.
    q1_normalized = normalize(q1);
    q2_normalized = normalize(q2);
    q3_normalized = normalize(q3);
    
    // Add elements to the out array.
    for (int i = 0; i < 3; i++) {
        q_out[i][0] = q1_normalized[i];
    }
    
    for (int i = 0; i < 3; i++) {
        q_out[i][1] = q2_normalized[i];
    }
    
    for (int i = 0; i < 3; i++) {
        q_out[i][2] = q3_normalized[i];
    }
    
    float place_holderR[3][3];                                                              // Create place_holder matrices –– makes it easier!
    float place_holderQ[3][3];
    float place_holderA[3][3];
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            place_holderQ[i][j] = q_out[i][j];
            place_holderA[i][j] = matrix[i][j];
        }
    }
    tranpose(place_holderQ);
    tranpose(place_holderA);                                                                // Transpose the final matrix.
    
    matrix_product(place_holderQ, place_holderA, place_holderR);                            // Multiply Q^T * A to return R (property of orthogonal matrices).

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            r_out[i][j] = place_holderR[i][j];
        }
    }

    return 0;
}

// QR method for approximating eigenvalues.
float * QR_Algo(float matrix[][3], int stepLimit, float eigenvalues[]) {
    float faux_matrix[3][3];                                                                // Create a dummy matrix that will change after every loop.
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            faux_matrix[i][j] = matrix[i][j];
        }
    }
    
    int i = 1;
    while (i <= (LIMIT - 1)) {                                                              // Run until limit is reached (-1)
        float q_temp[3][3];
        float r_temp[3][3];
        QR_Decomposition(faux_matrix, q_temp, r_temp);                                      // Compute the QR decomposition of the dummy matrix.
        matrix_product(r_temp, q_temp, faux_matrix);                                        // Reassign the dummy matrix
        i++;                                                                                // Increment step number.
        
    }
    
    for (int i = 0; i < 3; i++) {
        eigenvalues[i] = faux_matrix[i][i];                                                 // Get elements along the diagonal.
    }
    
    return 0;
}
