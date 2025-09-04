# Convenciones en C++

## Nombres de archivos
1. **Archivos fuente (.cpp):** Usar `snake_case` y nombres descriptivos.
   ```
   ordenamiento_rapido.cpp
   gestor_archivos.cpp
   ```

2. **Archivos de cabecera (.hpp):** También usar `snake_case` y reflejar el contenido o la clase principal.
   ```
   gestor_archivos.hpp
   usuario.hpp
   ```

3. **Archivos relacionados:** Si un archivo fuente y su cabecera están relacionados, deben compartir el mismo nombre base.
   ```
   gestor_archivos.cpp
   gestor_archivos.hpp
   ```

4. **Evitar nombres genéricos:** No usar nombres como `main.cpp` para archivos que no sean el punto de entrada del programa.

5. **Archivos de prueba:** Usar el sufijo `_test.cpp` para archivos de pruebas.
   ```
   gestor_archivos_test.cpp
   ```

## Variables
1. **Nombres descriptivos**: Los nombres deben describir claramente el propósito de la variable.
   ```cpp
   int total_usuarios; // Correcto
   int x;             // Evitar esto
   ```

2. **snake_case**:
   Usar guiones bajos para separar palabras.
    ```cpp
    int numero_usuarios_activos;
    ```

3. **Constantes**: Usar `UPPER_SNAKE_CASE` para constantes.
   ```cpp
   const int MAX_USUARIOS = 100;
   ```


## Clases
1. **PascalCase**: Usar mayúsculas al inicio de cada palabra.
   ```cpp
   class UsuarioActivo {
       // ...existing code...
   };
   ```

2. **Nombres sustantivos**: Usar nombres que representen entidades o conceptos.
   ```cpp
   class GestorDeArchivos; // Correcto
   class Procesar;         // Evitar nombres de acciones
   ```

3. **Miembros privados**: Usar un prefijo como `_` para distinguir miembros privados.
   ```cpp
   class Usuario {
   private:
       int _id;       // Prefijo '_'
   public:
       // Métodos públicos
   };
   ```

## Métodos
1. **Nombres descriptivos**: Los nombres de los métodos deben describir claramente su propósito o acción.
   ```cpp
   void calcularPromedio(); // Correcto
   void cp();               // Evitar esto
   ```

2. **CamelCase**: Usar `camelCase` para los nombres de los métodos, comenzando con minúscula y usando mayúsculas para separar palabras.
   ```cpp
   void obtenerDatosUsuario();
   ```

3. **Verbos al inicio**: Los nombres de los métodos deben comenzar con un verbo que indique la acción que realizan.
   ```cpp
   void guardarArchivo(); // Correcto
   void archivoGuardar(); // Evitar esto
   ```

4. **Prefijos para accesores y mutadores**:
   - Usar `get` para métodos que obtienen valores.
   - Usar `set` para métodos que asignan valores.
   ```cpp
   int getEdad();
   void setEdad(int edad);
   ```

5. **Métodos privados**: Si el método es privado, puedes usar un prefijo como `_` para distinguirlo.
   ```cpp
   class Usuario {
   private:
       void _validarDatos();
   public:
       void procesarUsuario();
   };
   ```


## Comentarios
1. **Comentarios de línea**: Usar `//` para comentarios breves.
   ```cpp
   int totalUsuarios = 0; // Inicializamos el contador
   ```

2. **Comentarios de bloque**: Usar `/* */` para comentarios más largos o explicativos.
   ```cpp
   /*
    * Este método calcula el total de usuarios activos
    * y retorna el resultado como un entero.
    */
   int calcularUsuariosActivos();
   ```

3. **Documentación**: Usar el estandar de Doxygen para documentar funciones, clases y métodos.
   ```cpp
   /**
    * Calcula el total de usuarios activos.
    * @param usuarios Lista de usuarios.
    * @return Número total de usuarios activos.
    */
   int calcularUsuariosActivos(vector<Usuario> usuarios);
   ```

4. **Evitar comentarios innecesarios**: No comentes lo obvio.
   ```cpp
   int x = 5; // Asignamos 5 a x (innecesario)
   ```

## Commits en GitHub

- `feat`: Para agregar una nueva funcionalidad o característica al proyecto.
  
- `build`: Cambios que afectan el sistema de compilación o dependencias externas (ej. cambios en el `Makefile`).
  
- `ci`: Cambios en archivos y scripts de configuración de integración continua.
  
- `docs`: Cambios en la documentación.
  
- `chore`: Otros cambios que no afectan el código fuente.
  
- `perf`: Cambios en el código que mejoran el rendimiento.
  
- `refactor`: Cambios en el código que no corrigen errores ni agregan características.
  
- `style`: Cambios que no afectan el significado del código (espacios en blanco, formato, puntos y comas, etc.).
  
- `test`: Agregar pruebas faltantes o corregir pruebas existentes.
