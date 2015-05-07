How to use the Blender Plugin to create an avango-application:

1. Get Blender (Tested in version 2.73a)
2. Add a link to the blender-addon to blender/*version*/scripts/addons
   Link has to be called "avango-blender"
   To start blender you'll need to set the environment variables for avango, for example with a startscript. (For the previewer) 

3. Create Application in Blender 
   For reference or starting point, load the ".blend" files in the examples directory

either:
4. Set blender-json-loader path in Blenders user preferences
5. Use the "start application" button/operator

or:
4. Export the avango-application via the export menu
5. Use the "start.sh" in the blender-json-loader to start the "*.avango" file


To add additional functionality, edit the "main.py" of the loader. 