# Carga de Modelos

Los modelos 3D son un conjunto de información de vértices. Estos pueden estar acompañados de información anexa, como color, textura, normal. Esto debe ser acondicionado y medido para ser introducido al buffer de la GPU, que va a devolver los elementos necesarios para llamar al espacio de memoria alojado, y que se ejecute el dibujado de esta estructura de información.

El formato más simple de lectura es el Obj, donde se nos entrega una estructura simple de puntos tridimencionales.

En la actualidad, existen múltiples formatos de información Objetos 3D, es por eso que se utilizará una librería para la carga de información, que acepte múltiples formatos.

La librería utilizada será Assimp.

Básicamente, el flujo de carga es el siguiente:

```
flowchart TD
    A[Proceso de Carga de Archivo 3D a GPU] --> B(Carga de Archivo por la librería)
    B --> C(Creación del Array de Vértices)
    C --> D(Separación de información de Vértice )
    D --> E(Creación de Buffer)
    E --> F(Pasar Artributos de vértices)
    F --> G(Crear índice de Buffers)
    G --> H(Cargar Buffer de vertices)
    H --> I(Cargar índice de Buffer)
    I --> J(Dibujado)
    J --> K(Bind de Textura)
    K --> L(Seleccionar Buffer)
    L --> J(Dibujar)
    J --> M(Limpieza)
    M --> N(Destrucción de los Buffers)
    N --> O(Destrucción de la Textura)
    O --> P(Reinicio del Modelo)

```
