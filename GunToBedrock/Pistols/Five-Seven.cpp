#include Minecraft.h
#include player.h
#include item.h
#include projectile.h
#include sound.h
#include entity.h

class FIVE_SEVEN : public Item {
public:
    FIVE_SEVEN() : Item(five_seven, ItemType::CUSTOM) {
        maxAmmo = 20;
        currentAmmo = maxAmmo;
        fireRate = 0.3f;
        lastFireTime = 0.0f;
    }

    void onRightClick(Player &player) override {
        float currentTime = Minecraft::getTime();
        if (currentAmmo <= 0) {
            player.sendMessage("Votre FIVE-SEVEN est vide. Rechargez.");
            return;
        }

        if (currentTime - lastFireTime < fireRate) {
            return;
        }

        shoot(player);
        currentAmmo--;
        lastFireTime = currentTime;

        if (currentAmmo == 0) {
            player.sendMessage("Votre FIVE-SEVEN est à court de munitions. Rechargez.");
        }
    }

    void reload(Player &player) {
        if (player.hasItem(ItemType::BULLET)) {
            int ammoToReload = maxAmmo - currentAmmo;
            currentAmmo += ammoToReload;
            player.removeItem(ItemType::BULLET, ammoToReload);
            player.sendMessage("Votre FIVE-SEVEN a été rechargé avec succès.");
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
        bullet.setDamage(10);
        bullet.setVelocity(player.getDirection() * 5.0f);
        player.playSound(SoundType::GUNSHOT);
        player.sendMessage("Tir. Le FIVE-SEVEN a tiré.");
    }
};

extern "C" void init() {
    Item::registerItem<FIVE_SEVEN>(five_seven);
}