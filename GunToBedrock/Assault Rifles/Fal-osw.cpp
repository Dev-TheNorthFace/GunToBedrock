#include "Minecraft.h"
#include "player.h"
#include "item.h"
#include "projectile.h"
#include "sound.h"
#include "entity.h"

class FALOSW : public Item {
public:
    FALOSW() : Item("fal_osw", ItemType::CUSTOM) {
        maxAmmo = 20;
        currentAmmo = maxAmmo;
    }

    void onRightClick(Player &player) override {
        if (currentAmmo <= 0) {
            player.sendMessage("Votre FAL-OSW est vide. Rechargez.");
            return;
        }

        shoot(player);
        currentAmmo--;

        if (currentAmmo == 0) {
            player.sendMessage("Votre FAL-OSW est à court de munitions. Rechargez.");
        }
    }

    void reload(Player &player) {
        if (player.hasItem(ItemType::ARROW)) {
            currentAmmo = maxAmmo;
            player.removeItem(ItemType::ARROW, maxAmmo);
            player.sendMessage("Votre FAL-OSW a été rechargé avec succès.");
        } else {
            player.sendMessage("Vous n'avez pas assez de munitions pour recharger.");
        }
    }

private:
    int maxAmmo;
    int currentAmmo;

    void shoot(Player &player) {
        Projectile &bullet = player.spawnProjectile(ProjectileType::SNOWBALL);
        bullet.setDamage(15);
        bullet.setVelocity(player.getDirection() * 2.5f);
        player.playSound(SoundType::SHOOT);
        player.sendMessage("Bang ! Le FAL-OSW a tiré.");
    }
};

extern "C" void init() {
    Item::registerItem<FALOSW>("fal_osw");
}