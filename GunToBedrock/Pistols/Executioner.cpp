#include Minecraft.h
#include player.h
#include item.h
#include projectile.h
#include sound.h
#include entity.h

class EXECUTIONER : public Item {
public:
    EXECUTIONER() : Item(executioner, ItemType::CUSTOM) {
        maxAmmo = 6;
        currentAmmo = maxAmmo;
        fireRate = 1.5f;
        lastFireTime = 0.0f;
    }

    void onRightClick(Player &player) override {
        float currentTime = Minecraft::getTime();
        if (currentAmmo <= 0) {
            player.sendMessage("Votre EXECUTIONER est vide. Rechargez.");
            return;
        }

        if (currentTime - lastFireTime < fireRate) {
            return;
        }

        shoot(player);
        currentAmmo--;
        lastFireTime = currentTime;

        if (currentAmmo == 0) {
            player.sendMessage("Votre EXECUTIONER est à court de munitions. Rechargez.");
        }
    }

    void reload(Player &player) {
        if (player.hasItem(ItemType::BULLET)) {
            int ammoToReload = maxAmmo - currentAmmo;
            currentAmmo += ammoToReload;
            player.removeItem(ItemType::BULLET, ammoToReload);
            player.sendMessage("Votre EXECUTIONER a été rechargé avec succès.");
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
        bullet.setVelocity(player.getDirection() * 3.0f);
        player.playSound(SoundType::GUNSHOT);
        player.sendMessage("Tir. L'EXECUTIONER a tiré.");
    }
};

extern "C" void init() {
    Item::registerItem<EXECUTIONER>(executioner);
}