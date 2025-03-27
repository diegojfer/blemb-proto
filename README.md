# blemb-proto

**`blemb-proto`** is a lightweight C library that implements a communication protocol designed for transmission over low-MTU transport layers, such as Bluetooth Low Energy (BLE). It enables sending large messages by splitting them into smaller packets and reassembling them on the receiving end.

## ✨ Features

- Message fragmentation and reassembly for low-MTU transports.
- Designed for embedded systems and constrained environments.
- Clean and portable C implementation.
- No external dependencies.
- CRC8 checksum for end-to-end message integrity.
- Minimal overhead, suitable for real-time use cases.

## 📦 Components

The library is composed of two main components:

### `protow` (Protocol Writer)

Responsible for preparing messages for transmission:

- Splits large messages into small chunks (packets).
- Provides a packet stream ready for transport over BLE or similar protocols.

Intended for use on the **sending** side.

### `protoh` (Protocol Handler)

Handles incoming data and reconstructs messages:

- Receives individual fragments and reassembles them.
- Detects and reports when a full message has been reconstructed.
- Supports basic error detection and recovery mechanisms.

Intended for use on the **receiving** side.

## 🚀 Basic Usage

This section shows how to use each component of **blemb-proto** individually.

### 📨 `protow` – Protocol Writer (Sender side)

```c
// Dummy writer function that would send a packet over BLE
void my_packet_writer(blemb_buffer_t packet) {
    // Send `packet.data` of size `packet.size` over your transport (e.g., BLE GATT)
    // For this example, we just print the size
    printf("Sending packet of size: %u\n", packet.size);
}

int main(int argc, char ** argv) {
    blemb_protow_context_t ctx = {
        .magic = 0xAB,                  // Custom protocol identifier
        .mtu = 20,                      // Example MTU size (e.g., BLE GATT characteristic)
        .writer = my_packet_writer,     // Function used to send each packet
    };

    blemb_byte_t message_data[] = { ... };

    blemb_buffer_t message = {
        .data = message_data,
        .size = sizeof(message_data),
    };

    if (blemb_protow_write(&ctx, message) == BLEMB_FALSE) {
        // Handle error
        printf("Failed to write message\n");
    }

    return EXIT_SUCCESS;
}
```

### 📥 `protoh` – Protocol Handler (Receiver side)

When a complete message is received:

- The `handler` function is called with the reassembled `blemb_buffer_t`.
- Optionally, a `validator` function can be provided. It will be called when a full message is detected. If it returns `BLEMB_FALSE`, the message is discarded.
- The `validator` function can be set to `NULL` if not needed.

> **Important:**
>
> - You **must call** `blemb_protoh_handle` for **each incoming packet**.
> - Packets must be received **in the correct order**; out-of-order delivery is not supported.
> - The field `.buffer_cur_size` **must be initialized to `0`** before the first use.
> - **Do not modify** `.buffer_cur_size` manually — it is managed internally by the library.

```c
// Called when a complete message has been reassembled
void my_message_handler(blemb_buffer_t message) {
    printf("Received complete message of size: %u\n", message.size);
}

// Optionally validate the message before accepting it
blemb_bool_t my_message_validator(blemb_buffer_t message) {
    // Example: only accept messages smaller than 100 bytes
    return message.size < 100 ? BLEMB_TRUE : BLEMB_FALSE;
}

int main(int argc, char ** argv) {
    blemb_byte_t buffer[128];  // Buffer for reassembly

    blemb_protoh_context_t ctx = {
        .magic = 0xAB,                      // Must match sender's magic
        .buffer_data = buffer,
        .buffer_cur_size = 0,              // MUST be 0 on startup
        .buffer_max_size = sizeof(buffer),
        .validator = my_message_validator, // Can be NULL
        .handler = my_message_handler
    };

    // Simulate receiving a BLE packet
    blemb_byte_t packet_0[] = { ... };
    blemb_buffer_t fragment_0 = {
        .data = packet_0,
        .size = sizeof(packet_0)
    };

    if (blemb_protoh_handle(&ctx, fragment_0) == BLEMB_FALSE) {
        // Handle error (e.g., invalid packet or overflow)
        printf("Error handling packet 0\n");

        return EXIT_FAILURE;
    }

    // Simulate receiving a BLE packet
    blemb_byte_t packet_1[] = { ... };
    blemb_buffer_t fragment_1 = {
        .data = packet_1,
        .size = sizeof(packet_1)
    };

    if (blemb_protoh_handle(&ctx, fragment_1) == BLEMB_FALSE) {
        // Handle error (e.g., invalid packet or overflow)
        printf("Error handling packet 0\n");

        return EXIT_FAILURE;
    }

    // Simulate receiving a BLE packet
    blemb_byte_t packet_2[] = { ... };
    blemb_buffer_t fragment_2 = {
        .data = packet_2,
        .size = sizeof(packet_2)
    };

    if (blemb_protoh_handle(&ctx, fragment_2) == BLEMB_FALSE) {
        // Handle error (e.g., invalid packet or overflow)
        printf("Error handling packet 0\n");

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
```

## ⚠️ Buffer Lifetime Warning

When using **blemb-proto**, the library may call user-provided callbacks such as:

- `handler` (from `protoh`)
- `validator` (from `protoh`)
- `writer` (from `protow`)

In all of these cases, a `blemb_buffer_t` is passed **by the system** to the implementer's function.

> ⚠️ **Important:** The buffer provided is **only valid during the execution** of the callback function.  
> After the function returns, the buffer is **no longer valid and may be freed or reused internally**.

If you need to retain the data **after the callback ends**, **you must copy it** into a memory buffer that you manage yourself.

### ✅ Safe:

```c
void my_message_handler(blemb_buffer_t msg) {
    blemb_byte_t * my_copy = malloc(msg.size);
    if (my_copy != NULL) {
        memcpy(my_copy, msg.data, msg.size);
        // Store or process `my_copy` as needed
    }
}
```

### ❌ Unsafe:

```c
blemb_byte_t * global_ptr;

void my_message_handler(blemb_buffer_t msg) {
    global_ptr = msg.data; // ⚠️ INVALID after function returns
}
```

## 🤝 Contributing

This project is open to contributions!

If you find a bug, unexpected behavior, or have a suggestion to improve **blemb-proto**, feel free to:

- Open an [issue](https://github.com/diegojfer/blemb-proto/issues)
- Submit a pull request
- Share feedback or ideas

Whether it's a fix, a new feature, or just documentation improvements — contributions are always welcome and appreciated.
