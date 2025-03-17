package sdu.revolution.server;

import java.net.SocketAddress;

public class PlayerEntity {
    public String name;
    public SocketAddress address;

    public PlayerEntity(String name, SocketAddress address) {
        this.name = name;
        this.address = address;
    }
}
