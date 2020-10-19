/**
 ** Max Mitchell & Jack Burns
 ** ppmtrans.c
 ** 20 February 2020
 **
 ** Purpose: Provides a system for rotating ppm images. Can rotate
 **          0, 90, 180, or 270 degrees, as well as flip horizontally
 **          and vertically.
 **/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"
#include "cputiming.h"

#define TRUE 0
#define FALSE 1


#define SET_METHODS(METHODS, MAP, WHAT) do {                    \
        methods = (METHODS);                                    \
        assert(methods != NULL);                                \
        map = methods->MAP;                                     \
        if (map == NULL) {                                      \
                fprintf(stderr, "%s does not support "          \
                                WHAT "mapping\n",               \
                                argv[0]);                       \
                exit(1);                                        \
        }                                                       \
} while (0)

static void
usage(const char *progname)
{
        fprintf(stderr, "Usage: %s [-rotate <angle>] "
                        "[-{row,col,block}-major] [filename]\n",
                        progname);
        exit(1);
}

/* @function: apply90
 * @purpose: apply function to be passed in to map function to rotate
 *           image 90 degrees clockwise. The way our apply functions
 *           work, we iterate over the 2D array which will be the finished,
 *           rotated, image. The original image is passed in via the closure
 *           variable.
 *
 * @postcondition: the value at a given index will be set to value in
 *                 ppm 90 degrees COUNTERclockwise to current position in
 *                 array2
 *
 * @parameters: 1) int col, the col in the array2 we are at
 *              2) int row, the row in the array2 we are at
 *              3) A2Methods_UArray2 array2, the array2 we are iterating over
 *              4) A2MEthods_Object *ptr, the element at the given row and col
                   in array2
 *              5) void *cl, the Pnm_ppm holding the original image we want to 
 *                 rotate
 *
 * @returns: none
 */
void apply90(int col, int row, A2Methods_UArray2 array2,
            A2Methods_Object *ptr, void *cl)
{
    (void) array2;
    assert(ptr != NULL);
    assert(cl != NULL);

    Pnm_ppm *ppm = cl;
    int oldRow = (*ppm)->height - col - 1;
    int oldCol = row;

    *(Pnm_rgb)ptr = *(Pnm_rgb)((*ppm)->methods->at((*ppm)->pixels, 
                                                     oldCol, oldRow)); 
}

/* @function: apply180
 * @purpose: apply function to be passed in to map function to rotate
 *           image 180 degrees. 
 *
 * @postcondition: the value at a given index will be set to value in
 *                 ppm 180 degrees from current position in array2
 *
 * @parameters: 1) int col, the col in the array2 we are at
 *              2) int row, the row in the array2 we are at
 *              3) A2Methods_UArray2 array2, the array2 we are iterating over
 *              4) A2MEthods_Object *ptr, the element at the given row and col
                   in array2
 *              5) void *cl, the Pnm_ppm holding the original image we want to 
 *                 rotate
 *
 * @returns: none
 */
void apply180(int col, int row, A2Methods_UArray2 array2,
            A2Methods_Object *ptr, void *cl)
{
    (void) array2;
    assert(ptr != NULL);
    assert(cl != NULL);

    Pnm_ppm *ppm = cl;
    int oldRow = (*ppm)->height - row - 1;
    int oldCol = (*ppm)->width - col - 1;

    *(Pnm_rgb)ptr = *(Pnm_rgb)((*ppm)->methods->at((*ppm)->pixels, 
                                                     oldCol, oldRow)); 
}

/* @function: apply270
 * @purpose: apply function to be passed in to map function to rotate
 *           image 270 degrees clockwise. 
 *
 * @postcondition: the value at a given index will be set to value in
 *                 ppm 270 degrees COUNTERclockwise from current position
 *                 in array2.
 *
 * @parameters: 1) int col, the col in the array2 we are at
 *              2) int row, the row in the array2 we are at
 *              3) A2Methods_UArray2 array2, the array2 we are iterating over
 *              4) A2MEthods_Object *ptr, the element at the given row and col
                   in array2
 *              5) void *cl, the Pnm_ppm holding the original image we want to 
 *                 rotate
 *
 * @returns: none
 */
void apply270(int col, int row, A2Methods_UArray2 array2,
            A2Methods_Object *ptr, void *cl)
{
    (void) array2;
    assert(ptr != NULL);
    assert(cl != NULL);

    Pnm_ppm *ppm = cl;
    int oldRow = col;
    int oldCol = (*ppm)->width - row - 1;

    *(Pnm_rgb)ptr = *(Pnm_rgb)((*ppm)->methods->at((*ppm)->pixels, 
                                                     oldCol, oldRow)); 
}

/* @function: applyFlipHorizontal
 * @purpose: apply function to be passed in to map function to flip
 *           image horizontally. 
 *
 * @postcondition: the value at a given index will be set to value in
 *                 ppm flipped across the horizontal axis
 *
 * @parameters: 1) int col, the col in the array2 we are at
 *              2) int row, the row in the array2 we are at
 *              3) A2Methods_UArray2 array2, the array2 we are iterating over
 *              4) A2MEthods_Object *ptr, the element at the given row and col
 *                 in array2
 *              5) void *cl, the Pnm_ppm holding the original image we want to 
 *                 flip
 *
 * @returns: none
 */
void applyFlipHorizontal(int col, int row, A2Methods_UArray2 array2,
            A2Methods_Object *ptr, void *cl)
{
    (void) array2;
    assert(ptr != NULL);
    assert(cl != NULL);

    Pnm_ppm *ppm = cl;
    int oldRow = (*ppm)->height - row - 1;
    int oldCol = col;

    *(Pnm_rgb)ptr = *(Pnm_rgb)((*ppm)->methods->at((*ppm)->pixels, 
                                                     oldCol, oldRow)); 
}

/* @function: applyFlipVertical
 * @purpose: apply function to be passed in to map function to flip
 *           image vertically. 
 *
 * @postcondition: the value at a given index will be set to value in
 *                 ppm flipped across the vertical axis
 *
 * @parameters: 1) int col, the col in the array2 we are at
 *              2) int row, the row in the array2 we are at
 *              3) A2Methods_UArray2 array2, the array2 we are iterating over
 *              4) A2MEthods_Object *ptr, the element at the given row and col
 *                 in array2
 *              5) void *cl, the Pnm_ppm holding the original image we want to 
 *                 flip
 *
 * @returns: none
 */
void applyFlipVertical(int col, int row, A2Methods_UArray2 array2,
            A2Methods_Object *ptr, void *cl)
{
    (void) array2;
    assert(ptr != NULL);
    assert(cl != NULL);

    Pnm_ppm *ppm = cl;
    int oldRow = row;
    int oldCol = (*ppm)->width - col - 1;

    *(Pnm_rgb)ptr = *(Pnm_rgb)((*ppm)->methods->at((*ppm)->pixels, 
                                                     oldCol, oldRow)); 
}

/* @function: newRotatedUArray2
 * @purpose: helper function to break up the code from main. Simply
 *           created new UArray2 in particular style
 *
 * @postcondition: new UArray2 is returned in proper format
 *
 * @parameters: 1) int rotation, the degrees we are rotating
 *              2) Pnm_ppm ppm, the image we are rotating
 *              3) A2Methods_T methods, the methods suite we are using
 *
 * @returns: A2Methods_UArray2, correctly formatted and initialized UArray2
 */
A2Methods_UArray2 newRotatedUArray2(int rotation, Pnm_ppm ppm, 
                                    A2Methods_T methods)
{
    int newWidth = ppm->width;
    int newHeight = ppm->height;
    /* if rotating 90 or 270, height and width of
     * image must be flipped 
     */
    if (rotation == 90 || rotation == 270) {
        newHeight = ppm->width;
        newWidth = ppm-> height;
    }
    return methods->new(newWidth, newHeight, sizeof(struct Pnm_rgb)); 
}

/* @function: alterImage
 * @purpose: helper function to break up the code from main. Handles
 *           calling proper rotation function given input.
 *
 * @postcondition: proper rotation function will be called
 *
 * @parameters: 1) int rotation, the rotation we are doing
 *              2) char *flip, the flip being called
 *              3) Pnm_ppm ppm, the image being rotated/flipped
 *              4) A2Methods_mapfun map, the map function that has been
 *                 determined by client
 *              5) A2Methods_UArray2 rotated, the uarray2 that will become
 *                 the fully rotated image.
 *
 * @returns: none
 */
void alterImage(int rotation, char *flip, Pnm_ppm ppm, A2Methods_mapfun map, 
                A2Methods_UArray2 rotated)
{
    switch (rotation) {
            case 90:
                map(rotated, apply90, &ppm);
                break;
            case 180:
                map(rotated, apply180, &ppm);
                break;
            case 270:
                map(rotated, apply270, &ppm);
                break;

            default:
                break;
        }

        if (strcmp(flip, "horizontal") == 0) {
            map(rotated, applyFlipHorizontal, &ppm);           
        }
        if (strcmp(flip, "vertical") == 0) {
            map(rotated, applyFlipVertical, &ppm);           
        }
}

int main(int argc, char *argv[]) 
{
        char *time_file_name = NULL;
        int   rotation       = 0;
        char *flip           = " ";
        int   i;

        int ppmOpen = FALSE;
        FILE *fp = NULL;
        Pnm_ppm ppm;

        /* default to UArray2 methods */
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods);

        /* default to best map */
        A2Methods_mapfun *map = methods->map_default; 
        assert(map);

        for (i = 1; i < argc; i++) {
                if (strcmp(argv[i], "-row-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_row_major, 
                    "row-major");
                } else if (strcmp(argv[i], "-col-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_col_major, 
                    "column-major");
                } else if (strcmp(argv[i], "-block-major") == 0) {
                        SET_METHODS(uarray2_methods_blocked, map_block_major,
                                    "block-major");
                } else if ((strcmp(argv[i], "-rotate") == 0)) {
                        if (!(i + 1 < argc)) {      /* no rotate value */
                                usage(argv[0]);
                        }
                        char *endptr;

                        rotation = strtol(argv[++i], &endptr, 10);
                        if (!(rotation == 0 || rotation == 90 ||
                            rotation == 180 || rotation == 270)) {
                                fprintf(stderr, 
                    "Rotation must be 0, 90 180 or 270\n");
                                usage(argv[0]);
                        }
                        if (!(*endptr == '\0')) {    /* Not a number */
                                usage(argv[0]);
                        }
                } else if ((strcmp(argv[i], "-flip") == 0)) {
                        if (!(i + 1 < argc)) {      /* no rotate value */
                                usage(argv[0]);
                        }

                        flip = argv[++i];
                        if (!(strcmp(flip, "horizontal") == 0 || 
                              strcmp(flip, "vertical") == 0)) {
                                fprintf(stderr, 
                    "Flip must be horizontal or vertical\n");
                                usage(argv[0]);
                        }
                } else if (strcmp(argv[i], "-time") == 0) {
                        time_file_name = argv[++i];
                } else if (*argv[i] == '-') {
                        fprintf(stderr, "%s: unknown option '%s'\n", argv[0],
                argv[i]);
                } else if (ppmOpen != TRUE) {
                        fp = fopen(argv[i], "r");
                        if (fp != NULL) {
                            ppm = Pnm_ppmread(fp, methods);
                        }
                        ppmOpen = TRUE;
                } else if (argc - i > 1) {
                        fprintf(stderr, "Too many arguments\n");
                        usage(argv[0]);
                } else {
                        break;
                }
        }

        /* if after all args still need image, take from stdin */
        if (ppmOpen != TRUE) {
            ppm = Pnm_ppmread(stdin, methods);
        }

        /* if no rotation or flip given, default to 0 degrees */
        if (rotation == 0 && strcmp(flip, " ") == 0) {
            Pnm_ppmwrite(stdout, ppm);
            Pnm_ppmfree(&ppm);
            if (fp != NULL) {
                fclose(fp);
            } 
            exit(EXIT_SUCCESS);
        }

        A2Methods_UArray2 rotated = newRotatedUArray2(rotation, ppm, methods);

        /* if -time has been invoked, start timing */
        FILE *output;
        CPUTime_T timer;
        double timeTot;
        if (time_file_name != NULL) {
            output = fopen(time_file_name, "w");
            timer = CPUTime_New();
            fprintf(output, "Time Data for Image of height: %d \
and width: %d\n", ppm->height, ppm->width); 
            CPUTime_Start(timer);
        }
        else {
            (void) output;
            (void) timer;
            (void) timeTot;
        }

        /*................... PERFORM OPERATION ...................*/

        alterImage(rotation, flip, ppm, map, rotated);

        /* if -time has been invoked, stop timing */
        if (time_file_name != NULL) {
            timeTot = CPUTime_Stop(timer);
            fprintf(output, "Total time for %d degree rotation: %lf \
nanoseconds\n", rotation, timeTot);
            fprintf(output, "Time for each pixel: %lf nanoseconds\n", 
                    timeTot / (ppm->width * ppm->height));   
            CPUTime_Free(&timer);
            fclose(output);
        }
        

        /* copy rotated image back to Pnm_ppm and write out */
        A2Methods_UArray2 pixels = ppm->pixels;
        methods->free(&pixels);
        ppm->pixels = rotated;
        ppm->height = methods->height(rotated);
        ppm->width = methods->width(rotated);
        Pnm_ppmwrite(stdout, ppm);

        Pnm_ppmfree(&ppm);
        if (fp != NULL) {
            fclose(fp);
        } 

        exit(EXIT_SUCCESS);  
}            
