#include "Minecraft.h"
#include "player.h"
#include "item.h"
#include "projectile.h"
#include "sound.h"
#include "entity.h"
#include "timer.h"

class Type25 : public Item {
public:
    Type25() : Item("type_25", ItemType::CUSTOM) {
        maxAmmo = 30;
        currentAmmo = maxAmmo;
        isShooting = false;
    }

    void onRightClickHold(Player &player) override {
        if (currentAmmo <= 0) {
            player.sendMessage("Le Type 25 est vide. Rechargez.");
            return;
        }

        if (!isShooting) {
            isShooting = true;
            startAutomaticFire(player);
        }
    }

    void onRightClickRelease(Player &player) override {
        isShooting = false;
    }

    void reload(Player &player) {
        if (player.hasItem(ItemType::ARROW)) {
            currentAmmo = maxAmmo;
            player.removeItem(ItemType::ARROW, maxAmmo);
            player.sendMessage("Votre Type 25 a été rechargé.");
        } else {
            player.sendMessage("Vous n'avez pas de munitions pour recharger.");
        }
    }

private:
    int maxAmmo;
    int currentAmmo;
    bool isShooting;

    void startAutomaticFire(Player &player) {
        Timer::scheduleRepeating([this, &player]() {
            if (!isShooting || currentAmmo <= 0) {
                isShooting = false;
                return;
            }
            shoot(player);
            currentAmmo--;

            if (currentAmmo == 0) {
                player.sendMessage("Le Type 25 est vide. Rechargez avec des munitions.");
                isShooting = false;
            }
        }, 100);
    }

    void shoot(Player &player) {
        Projectile &bullet = player.spawnProjectile(ProjectileType::SNOWBALL);
        bullet.setDamage(8);
        player.playSound(SoundType::SHOOT);
        player.sendMessage("Bang !");
    }
};

extern "C" void init() {
    Item::registerItem<Type25>("type_25");
}