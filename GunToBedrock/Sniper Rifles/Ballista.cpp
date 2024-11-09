#include Minecraft.h
#include player.h
#include item.h
#include projectile.h
#include sound.h
#include entity.h

class Ballista : public Item {
public:
    Ballista() : Item(ballista, ItemType::CUSTOM) {
        maxAmmo = 5;
        currentAmmo = maxAmmo;
        fireRate = 4.0f;
        lastFireTime = 0.0f;
    }

    void onRightClick(Player &player) override {
        float currentTime = Minecraft::getTime();
        if (currentAmmo <= 0) {
            player.sendMessage("Votre Ballista est vide. Rechargez.");
            return;
        }

        if (currentTime - lastFireTime < fireRate) {
            return;
        }

        shoot(player);
        currentAmmo--;
        lastFireTime = currentTime;

        if (currentAmmo == 0) {
            player.sendMessage("Votre Ballista est à court de munitions. Rechargez.");
        }
    }

    void reload(Player &player) {
        if (player.hasItem(ItemType::ARROW)) {
            int ammoToReload = maxAmmo - currentAmmo;
            currentAmmo += ammoToReload;
            player.removeItem(ItemType::ARROW, ammoToReload);
            player.sendMessage("Votre Ballista a été rechargée avec succès.");
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
        Projectile &bolt = player.spawnProjectile(ProjectileType::ARROW);
        bolt.setDamage(100);
        bolt.setVelocity(player.getDirection() * 5.0f);
        player.playSound(SoundType::GUNSHOT);
        player.sendMessage("Boom ! La Ballista a tiré.");
    }
};

extern "C" void init() {
    Item::registerItem<Ballista>(ballista);
}