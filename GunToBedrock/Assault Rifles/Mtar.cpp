#include "Minecraft.h"
#include "player.h"
#include "item.h"
#include "projectile.h"
#include "sound.h"
#include "entity.h"

class MTAR : public Item {
public:
    MTAR() : Item("mtar", ItemType::CUSTOM) {
        maxAmmo = 30;
        currentAmmo = maxAmmo;
        burstMode = false;
    }

    void onRightClick(Player &player) override {
        if (currentAmmo <= 0) {
            player.sendMessage("La MTAR est vide. Rechargez.");
            return;
        }

        if (burstMode) {
            shootBurst(player);
        } else {
            shootSingle(player);
        }

        currentAmmo--;

        if (currentAmmo == 0) {
            player.sendMessage("La MTAR est vide. Rechargez avec des munitions.");
        }
    }

    void reload(Player &player) {
        if (player.hasItem(ItemType::ARROW)) {
            currentAmmo = maxAmmo;
            player.removeItem(ItemType::ARROW, maxAmmo);
            player.sendMessage("Votre MTAR a été rechargée.");
        } else {
            player.sendMessage("Vous n'avez pas de munitions pour recharger.");
        }
    }

    void toggleBurstMode(Player &player) {
        burstMode = !burstMode;
        if (burstMode) {
            player.sendMessage("Mode rafale activé.");
        } else {
            player.sendMessage("Mode semi-automatique activé.");
        }
    }

private:
    int maxAmmo;
    int currentAmmo;
    bool burstMode;

    void shootSingle(Player &player) {
        Projectile &bullet = player.spawnProjectile(ProjectileType::SNOWBALL);
        bullet.setDamage(10);
        player.playSound(SoundType::SHOOT);
        player.sendMessage("Bang");
    }

    void shootBurst(Player &player) {
        for (int i = 0; i < 3; ++i) {
            shootSingle(player);
        }
        player.sendMessage("Rafale");
    }
};

extern "C" void init() {
    Item::registerItem<MTAR>("mtar");
}