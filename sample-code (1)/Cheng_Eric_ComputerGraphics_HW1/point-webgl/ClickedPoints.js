// ClickedPints.js (c)
// Vertex shader program

/**
 * @author Eric Cheng
 * @class CSCI 168 Computer Graphics
 * 
 * This program utilizes the Bresenham Line algorithm
 * to generate a line given two points. The algorithm
 * accepts many cases of the endpoints with different
 * coordinates and slope functions. This code utilizes
 * the basic sample code provided from the ClickedPoints.js
 * file and the HelloTriangle.js triangle in order to
 * set up the canvas and the points drawn on the canvas.
 */
let VSHADER_SOURCE =
  'attribute vec4 a_Position;\n' +
  'void main() {\n' +
  '  gl_Position = a_Position;\n' +
  '  gl_PointSize = 10.0;\n' +
  '}\n';

// Fragment shader program
let FSHADER_SOURCE =
  'void main() {\n' +
  '  gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n' +
  '}\n';

function main() {
  // Retrieve <canvas> element
  let canvas = document.getElementById('webgl');

  // Get the rendering context for WebGL
  let gl = getWebGLContext(canvas);
  if (!gl) {
    console.log('Failed to get the rendering context for WebGL');
    return;
  }

  // Initialize shaders
  if (!initShaders(gl, VSHADER_SOURCE, FSHADER_SOURCE)) {
    console.log('Failed to intialize shaders.');
    return;
  }

  // Get the storage location of a_Position
  let a_Position = gl.getAttribLocation(gl.program, 'a_Position');
  if (a_Position < 0) {
    console.log('Failed to get the storage location of a_Position');
    return;
  }

  // Register function (event handler) to be called on a mouse press
  canvas.onmousedown = function(ev){ click(ev, gl, canvas, a_Position); };

  // Specify the color for clearing <canvas>
  gl.clearColor(0.0, 0.0, 0.0, 1.0);

  // Clear <canvas>
  gl.clear(gl.COLOR_BUFFER_BIT);
}

//Used to test the function
function main() {
  // Retrieve <canvas> element
  var canvas = document.getElementById('webgl');

  // Get the rendering context for WebGL
  var gl = getWebGLContext(canvas);
  if (!gl) {
    console.log('Failed to get the rendering context for WebGL');
    return;
  }

  // Initialize shaders
  if (!initShaders(gl, VSHADER_SOURCE, FSHADER_SOURCE)) {
    console.log('Failed to intialize shaders.');
    return;
  }

  // Write the positions of vertices to a vertex shader
  var n = initVertexBuffers(gl);
  if (n < 0) {
    console.log('Failed to set the positions of the vertices');
    return;
  }

  // Specify the color for clearing <canvas>
  gl.clearColor(0, 0, 0, 1);

  // Clear <canvas>
  gl.clear(gl.COLOR_BUFFER_BIT);

  // Draw the line using bresenham's algorithm
  gl.drawArrays(gl.POINTS, 0, n);
  console.log("Printing Points");

  gl.drawArrays(gl.LINES, 0, n);
  console.log("Printing Line");
}

function initVertexBuffers(gl) {

  let x = 0;
  let x1 = -0.5;

  let y = 0;
  let y1 = -0.5;

  var vertices = new Float32Array([
    x, y,   x1, y1 ]);

  bresenham(x, y, x1, y1);
  var n = 2; // The number of vertices

  // Create a buffer object
  var vertexBuffer = gl.createBuffer();
  if (!vertexBuffer) {
    console.log('Failed to create the buffer object');
    return -1;
  }

  // Bind the buffer object to target
  gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);
  // Write date into the buffer object
  gl.bufferData(gl.ARRAY_BUFFER, vertices, gl.STATIC_DRAW);

  var a_Position = gl.getAttribLocation(gl.program, 'a_Position');
  if (a_Position < 0) {
    console.log('Failed to get the storage location of a_Position');
    return -1;
  }
  // Assign the buffer object to a_Position variable
  gl.vertexAttribPointer(a_Position, 2, gl.FLOAT, false, 0, 0);

  // Enable the assignment to a_Position variable
  gl.enableVertexAttribArray(a_Position);

  return n;
}

/**
let g_points = []; // The array for the position of a mouse press
function click(ev, gl, canvas, a_Position) {
  let x = ev.clientX; // x coordinate of a mouse pointer
  let y = ev.clientY; // y coordinate of a mouse pointer
  let rect = ev.target.getBoundingClientRect() ;

  x = ((x - rect.left) - canvas.width/2)/(canvas.width/2);
  y = (canvas.height/2 - (y - rect.top))/(canvas.height/2);
  
  // Store the coordinates to g_points array
  g_points.push(x); g_points.push(y);

  // Clear <canvas>
  gl.clear(gl.COLOR_BUFFER_BIT);

  let len = g_points.length;
  for(let i = 0; i < len; i += 2) {
    // Pass the position of a point to a_Position letiable
    gl.vertexAttrib3f(a_Position, g_points[i], g_points[i+1], 0.0);
    console.log("drawing", i);
    
    // Draw
    gl.drawArrays(gl.POINTS, 0, 1);
    console.log("Printing Points");
  }
}
*/

/**
 * @param x1 = The x coordinate of the first endpoint
 * @param y1 = The y coordinate of the first endpoint
 * @param x2 = The x coordinate of the second endpoint
 * @param y2 = Theyx coordinate of the second endpoint
 * 
 * This is the Bresenham algorithm used to draw a line between
 * two given endpoints given their vertices. The parameters
 * given are the vertices of the two endpoints, and will be
 * used to calculate howthe line will be drawed.
 */
let x1, x2, y1, y2;
function bresenham(x1, y1, x2, y2)
{
  let x, y;

  //Find the change in x and y values of the vertices between the
  //two endpoints
  let dx = Math.abs(x2 - x1); 
  let dy = Math.abs(y2 - y1);

  //Store the initial x and y coordinates in a new variable
  x = x1;
  y = y1;

  //Use integer calculation to find the difference
  let d = 2 * dy - dx;

  //This while loop makes sure it runs through all possibilities
  //between the two endpoints
  while( x < x1 )
  {
    if(d >= 0)
    {
      //Push the points and increment y value because
      //it is closer to the top
      g_points.push(x); g_points.push(y);
      y++;
      p += 2 * dy;
    }
    else  
    {
      //Keep regular y value
      g_points.push(x); g_points.push(y);
      p += 2 * dy;
    }
    x++;
  }
}