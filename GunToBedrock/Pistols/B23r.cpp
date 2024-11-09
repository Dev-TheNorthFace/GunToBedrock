#include Minecraft.h
#include player.h
#include item.h
#include projectile.h
#include sound.h
#include entity.h

class B23R : public Item {
public:
    B23R() : Item(b23r, ItemType::CUSTOM) {
        maxAmmo = 15;
        currentAmmo = maxAmmo;
        fireRate = 0.4f;
        lastFireTime = 0.0f;
    }

    void onRightClick(Player &player) override {
        float currentTime = Minecraft::getTime();
        if (currentAmmo <= 0) {
            player.sendMessage("Votre B23R est vide. Rechargez.");
            return;
        }

        if (currentTime - lastFireTime < fireRate) {
            return;
        }

        shoot(player);
        currentAmmo--;
        lastFireTime = currentTime;

        if (currentAmmo == 0) {
            player.sendMessage("Votre B23R est à court de munitions. Rechargez.");
        }
    }

    void reload(Player &player) {
        if (player.hasItem(ItemType::BULLET)) {
            int ammoToReload = maxAmmo - currentAmmo;
            currentAmmo += ammoToReload;
            player.removeItem(ItemType::BULLET, ammoToReload);
            player.sendMessage("Votre B23R a été rechargé avec succès.");
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
        bullet.setDamage(12);
        bullet.setVelocity(player.getDirection() * 6.0f);
        player.playSound(SoundType::GUNSHOT);
        player.sendMessage("Tir. Le B23R a tiré.");
    }
};

extern "C" void init() {
    Item::registerItem<B23R>(b23r);
}