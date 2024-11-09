#include Minecraft.h
#include player.h
#include item.h
#include projectile.h
#include sound.h
#include entity.h

class R870MCS : public Item {
public:
    R870MCS() : Item(r870_mcs, ItemType::CUSTOM) {
        maxAmmo = 8;
        currentAmmo = maxAmmo;
        fireRate = 1.0f; 
        lastFireTime = 0.0f;
    }

    void onRightClick(Player &player) override {
        float currentTime = Minecraft::getTime();
        if (currentAmmo <= 0) {
            player.sendMessage("Votre R870 MCS est vide. Rechargez.");
            return;
        }

        if (currentTime - lastFireTime < fireRate) {
            player.sendMessage("Veuillez attendre avant de tirer à nouveau.");
            return;
        }

        shoot(player);
        currentAmmo--;
        lastFireTime = currentTime;

        if (currentAmmo == 0) {
            player.sendMessage("Votre R870 MCS est à court de munitions. Rechargez.");
        }
    }

    void reload(Player &player) {
        if (player.hasItem(ItemType::SHOTGUN_SHELL)) {
            currentAmmo = maxAmmo;
            player.removeItem(ItemType::SHOTGUN_SHELL, maxAmmo);
            player.sendMessage("Votre R870 MCS a été rechargé avec succès.");
        } else {
            player.sendMessage("Vous n'avez pas assez de munitions pour recharger.");
        }
    }

private:
    int maxAmmo;
    int currentAmmo;
    float fireRate;
    float lastFireTime;

    void shoot(Player &player) {
        Projectile &bullet = player.spawnProjectile(ProjectileType::SNOWBALL);
        bullet.setDamage(30); 
        bullet.setVelocity(player.getDirection() * 2.5f); 
        player.playSound(SoundType::SHOTGUN_FIRE);
        player.sendMessage("Boom. Le R870 MCS a tiré.");
    }
};

extern "C" void init() {
    Item::registerItem<R870MCS>(r870_mcs);
}