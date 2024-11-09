#include "Minecraft.h"
#include "player.h"
#include "item.h"
#include "projectile.h"
#include "sound.h"
#include "entity.h"

class SCARH : public Item {
public:
    SCARH() : Item("scar_h", ItemType::CUSTOM) {
        maxAmmo = 30;
        currentAmmo = maxAmmo;
    }

    void onRightClick(Player &player) override {
        if (currentAmmo <= 0) {
            player.sendMessage("Votre SCAR-H est vide. Rechargez.");
            return;
        }

        shoot(player);
        currentAmmo--;

        if (currentAmmo == 0) {
            player.sendMessage("Votre SCAR-H est à court de munitions. Rechargez.");
        }
    }

    void reload(Player &player) {
        if (player.hasItem(ItemType::BULLET)) {
            currentAmmo = maxAmmo;
            player.removeItem(ItemType::BULLET, maxAmmo);
            player.sendMessage("Votre SCAR-H a été rechargé avec succès.");
        } else {
            player.sendMessage("Vous n'avez pas assez de munitions pour recharger.");
        }
    }

private:
    int maxAmmo;
    int currentAmmo;

    void shoot(Player &player) {
        Projectile &bullet = player.spawnProjectile(ProjectileType::SNOWBALL);
        bullet.setDamage(20);
        bullet.setVelocity(player.getDirection() * 3.0f);
        player.playSound(SoundType::SHOOT);
        player.sendMessage("Bang ! La SCAR-H a tiré.");
    }
};

extern "C" void init() {
    Item::registerItem<SCARH>("scar_h");
}