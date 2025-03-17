package sdu.revolution.client.engine.gui.info;

import sdu.revolution.client.Main;

import java.text.SimpleDateFormat;
import java.util.Date;

public class InfoLibrary {
    public static String getGUIStatus() {
        return "Name: " + Main.logic.player.name +
                "  |  Time: " + new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date()) +
                "  |  Funds: " + Main.logic.player.balance;
    }

    public static String getTurnStatus() {
        if (Main.logic.player.canExecute) {
            return "You";
        } else {
            return "Opponent";
        }
    }
}
