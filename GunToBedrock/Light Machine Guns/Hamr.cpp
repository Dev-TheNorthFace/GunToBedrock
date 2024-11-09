#include Minecraft.h
#include player.h
#include item.h
#include projectile.h
#include sound.h
#include entity.h

class HAMR : public Item {
public:
    HAMR() : Item(hamr, ItemType::CUSTOM) {
        maxAmmo = 200;
        currentAmmo = maxAmmo;
        fireRate = 0.1f;
        lastFireTime = 0.0f;
    }

    void onRightClick(Player &player) override {
        float currentTime = Minecraft::getTime();
        if (currentAmmo <= 0) {
            player.sendMessage("Votre HAMR est vide. Rechargez.");
            return;
        }

        if (currentTime - lastFireTime < fireRate) {
            return;
        }

        shoot(player);
        currentAmmo--;
        lastFireTime = currentTime;

        if (currentAmmo == 0) {
            player.sendMessage("Votre HAMR est à court de munitions. Rechargez.");
        }
    }

    void reload(Player &player) {
        if (player.hasItem(ItemType::BULLET)) {
            int ammoToReload = maxAmmo - currentAmmo;
            currentAmmo += ammoToReload;
            player.removeItem(ItemType::BULLET, ammoToReload);
            player.sendMessage("Votre HAMR a été rechargé avec succès.");
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
        for (int i = 0; i < 5; ++i) {
            Projectile &bullet = player.spawnProjectile(ProjectileType::SNOWBALL);
            bullet.setDamage(12);
            bullet.setVelocity(player.getDirection() * 5.0f);
            player.playSound(SoundType::GUNSHOT);
        }
        player.sendMessage("Ratata. Le HAMR a tiré.");
    }
};

extern "C" void init() {
    Item::registerItem<HAMR>(hamr);
}