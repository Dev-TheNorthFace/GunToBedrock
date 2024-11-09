#include "Minecraft.h"
#include "player.h"
#include "item.h"
#include "projectile.h"
#include "sound.h"
#include "entity.h"

class SWAT556 : public Item {
public:
    SWAT556() : Item("swat_556", ItemType::CUSTOM) {
        maxAmmo = 20;
        currentAmmo = maxAmmo;
    }

    void onRightClick(Player &player) override {
        if (currentAmmo <= 0) {
            player.sendMessage("Le SWAT-556 est vide. Rechargez.");
            return;
        }

        shoot(player);
        currentAmmo--;

        if (currentAmmo == 0) {
            player.sendMessage("Le SWAT-556 est vide. Rechargez avec des munitions.");
        }
    }

    void reload(Player &player) {
        if (player.hasItem(ItemType::ARROW)) {
            currentAmmo = maxAmmo;
            player.removeItem(ItemType::ARROW, maxAmmo);
            player.sendMessage("Votre SWAT-556 a été rechargé.");
        } else {
            player.sendMessage("Vous n'avez pas de munitions pour recharger.");
        }
    }

private:
    int maxAmmo;
    int currentAmmo;

    void shoot(Player &player) {
        Projectile &bullet = player.spawnProjectile(ProjectileType::SNOWBALL);
        bullet.setDamage(12);
        bullet.setVelocity(player.getDirection() * 2.0f);
        player.playSound(SoundType::SHOOT);
        player.sendMessage("Bang !");
    }
};

extern "C" void init() {
    Item::registerItem<SWAT556>("swat_556");
}