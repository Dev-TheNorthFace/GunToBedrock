#include "Minecraft.h"
#include "player.h"
#include "item.h"
#include "projectile.h"
#include "sound.h"
#include "entity.h"

class M27 : public Item {
public:
    M27() : Item("m27", ItemType::CUSTOM) {
        maxAmmo = 30;
        currentAmmo = maxAmmo;
        isShooting = false;
    }

    void onRightClick(Player &player) override {
        if (currentAmmo <= 0) {
            player.sendMessage("Votre M27 est vide. Rechargez.");
            return;
        }

        isShooting = true;
        shoot(player);
    }

    void onRightRelease(Player &player) override {
        isShooting = false;
    }

    void reload(Player &player) {
        if (player.hasItem(ItemType::ARROW)) {
            currentAmmo = maxAmmo;
            player.removeItem(ItemType::ARROW, maxAmmo);
            player.sendMessage("Votre M27 a été rechargé.");
        } else {
            player.sendMessage("Vous n'avez pas de munitions pour recharger.");
        }
    }

private:
    int maxAmmo;
    int currentAmmo;
    bool isShooting;

    void shoot(Player &player) {
        if (isShooting && currentAmmo > 0) {
            Projectile &bullet = player.spawnProjectile(ProjectileType::SNOWBALL);
            bullet.setDamage(10);
            bullet.setVelocity(player.getDirection() * 2.0f);
            player.playSound(SoundType::SHOOT);
            currentAmmo--;

            if (currentAmmo == 0) {
                player.sendMessage("Le M27 est vide ! Rechargez.");
                isShooting = false;
            }
        }
    }
};

extern "C" void init() {
    Item::registerItem<M27>("m27");
}