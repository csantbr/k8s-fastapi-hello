FROM gcc:13-bookworm AS builder

WORKDIR /build
COPY main.cpp .
RUN g++ -o server main.cpp -lpthread -O2 -static-libstdc++ -static-libgcc

FROM debian:bookworm-slim

WORKDIR /app
COPY --from=builder /build/server .

EXPOSE 8000

CMD ["./server"]
