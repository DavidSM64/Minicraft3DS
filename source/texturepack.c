#include "texturepack.h"

#define dir_delimter '/'
#define MAX_FILENAME 256
#define READ_SIZE 9216


void toLowerString(char * str){
    int i;
    for (i = 0; str[i] != '\0'; i++)str[i] = (char)tolower((unsigned char)str[i]);
}

int getTexturePackComment(char * filename, char * cmmtBuf){

    // Open the zip file
    unzFile *zipfile = unzOpen(filename);
    if ( zipfile == NULL ) return 1; // Error: ZipFile could not be opened.
    
    // Get info about the zip file
    unz_global_info global_info;
    if (unzGetGlobalInfo(zipfile, &global_info ) != UNZ_OK )
    {
        unzClose( zipfile );
        return 2; // Error: Could not read global info
    }
    
    unzGetGlobalComment(zipfile, cmmtBuf, 58);

    unzClose( zipfile );

    return 0;
}

int loadTexturePack(char * filename){
    
    bool useDefaultIcons = true;
    bool useDefaultFont = true;
    bool useDefaultBottom = true;

    // Open the zip file
    unzFile *zipfile = unzOpen(filename);
    if ( zipfile == NULL ) return 1; // Error: ZipFile could not be opened.
    
    // Get info about the zip file
    unz_global_info global_info;
    if (unzGetGlobalInfo(zipfile, &global_info ) != UNZ_OK )
    {
        unzClose( zipfile );
        return 2; // Error: Could not read global info
    }

    // Buffer to hold data read from the zip file.
    char read_buffer[ READ_SIZE ];

    // Loop to extract all files
    uLong i;
    for ( i = 0; i < global_info.number_entry; ++i )
    {
        // Get info about current file.
        unz_file_info file_info;
        char filename[ MAX_FILENAME ];
        if (unzGetCurrentFileInfo(zipfile,&file_info,filename,MAX_FILENAME,NULL, 0, NULL, 0 ) != UNZ_OK ){
            unzClose( zipfile );
            return 3; // Error: Could not read file info
        }

        // Check if this entry is NOT a directory or file.
        const size_t filename_length = strlen( filename );
        if ( filename[ filename_length-1 ] != dir_delimter ){
            if ( unzOpenCurrentFile( zipfile ) != UNZ_OK )
            {
                unzClose( zipfile );
                return 4;
            }

            // Open a file to write out the data.
            FILE * out = fopen(filename, "wb" );
            if ( out == NULL )
            {
                unzCloseCurrentFile( zipfile );
                unzClose( zipfile );
                return 5;
            }

            int error = UNZ_OK;
            do    
            {
                error = unzReadCurrentFile( zipfile, read_buffer, READ_SIZE );
                if ( error < 0 )
                {
                    //printf( "error %d\n", error );
                    unzCloseCurrentFile( zipfile );
                    unzClose( zipfile );
                    return 6;
                }

                // Write data to file.
                if ( error > 0 )
                {
                    fwrite( read_buffer, error, 1, out ); // You should check return of fwrite...
                }
            } while ( error > 0 );

            fclose(out);
            
            char lowerFilename[MAX_FILENAME];
            strcpy(lowerFilename,filename);
            toLowerString(lowerFilename);
            
            if(strcmp(lowerFilename,"icons.png") == 0){
                if(sfil_load_PNG_file(filename, SF2D_PLACE_RAM) == NULL){
                    unzCloseCurrentFile( zipfile );
                    unzClose( zipfile );
                    return 7;
                }
	           icons = sfil_load_PNG_file(filename, SF2D_PLACE_RAM);
	           dirtColor[0] = SWAP_UINT32(sf2d_get_pixel(icons, 16, 0)); 
	           dirtColor[1] = SWAP_UINT32(sf2d_get_pixel(icons, 16, 1)); 
	           dirtColor[2] = SWAP_UINT32(sf2d_get_pixel(icons, 16, 2)); 
	           dirtColor[3] = SWAP_UINT32(sf2d_get_pixel(icons, 16, 3)); 
	           dirtColor[4] = SWAP_UINT32(sf2d_get_pixel(icons, 16, 4)); 
	           
	           useDefaultIcons = false;

            } else if(strcmp(lowerFilename,"font.png") == 0){
                if(sfil_load_PNG_file(filename, SF2D_PLACE_RAM) == NULL){
                    unzCloseCurrentFile( zipfile );
                    unzClose( zipfile );
                    return 7;
                }
	           font = sfil_load_PNG_file(filename, SF2D_PLACE_RAM);
	           useDefaultFont = false;
            } else if(strcmp(lowerFilename,"bottombg.png") == 0){
                if(sfil_load_PNG_file(filename, SF2D_PLACE_RAM) == NULL){
                    unzCloseCurrentFile( zipfile );
                    unzClose( zipfile );
                    return 7;
                }
	           bottombg = sfil_load_PNG_file(filename, SF2D_PLACE_RAM);
	           useDefaultBottom = false;
            }
            
	        remove(filename);
        }

        unzCloseCurrentFile( zipfile );

        // Go the the next entry listed in the zip file.
        if ( ( i+1 ) < global_info.number_entry )
        {
            if ( unzGoToNextFile( zipfile ) != UNZ_OK )
            {
                unzClose( zipfile );
                return 7;
            }
        }
    }
    
    if(useDefaultIcons){
        icons = sfil_load_PNG_buffer(icons2_png, SF2D_PLACE_RAM);
        dirtColor[0] = SWAP_UINT32(sf2d_get_pixel(icons, 16, 0)); 
        dirtColor[1] = SWAP_UINT32(sf2d_get_pixel(icons, 16, 1)); 
        dirtColor[2] = SWAP_UINT32(sf2d_get_pixel(icons, 16, 2)); 
        dirtColor[3] = SWAP_UINT32(sf2d_get_pixel(icons, 16, 3)); 
        dirtColor[4] = SWAP_UINT32(sf2d_get_pixel(icons, 16, 4));     
    }
    if(useDefaultFont) font = sfil_load_PNG_buffer(Font_png, SF2D_PLACE_RAM);
    if(useDefaultBottom) bottombg = sfil_load_PNG_buffer(bottombg_png, SF2D_PLACE_RAM);

    unzClose( zipfile );

    return 0;
}
