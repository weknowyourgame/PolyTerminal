# Backend API - gRPC Server

This is a gRPC starter backend for PolyTerminal.

## Structure

- `main.cpp` - The gRPC server implementation
- `helloworld.proto` - Protocol buffer definition

## Building

The project uses CMake to compile the proto files and build the server:

```bash
cd build
cmake ..
make BackendApi
```

## Running

```bash
./BackendApi
```

The server will start on `0.0.0.0:8888`.

## Proto File

The `helloworld.proto` file defines a simple greeting service:
- `SayHello` - Takes a name and returns a greeting message

To modify the API, edit `helloworld.proto` and rebuild.

